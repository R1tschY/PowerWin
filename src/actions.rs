use std::borrow::Cow;
use std::cell::RefCell;
use std::collections::HashMap;
use std::io;
use std::rc::Rc;

use lazy_static::lazy_static;
use lightports::Result;
use lightports_gui::app::app_instance;
use lightports_gui::extra::hotkey::HotKey;
use lightports_gui::sys::*;
use lightports_gui::user_control::*;
use lightports_gui::usr_ctrl::UsrCtrl;
use winapi::shared::windef::HWND;
use winapi::um::winuser::WS_POPUP;
use winapi::um::winuser::WS_EX_TOOLWINDOW;
use winapi::um::winuser::WM_DESTROY;
use winapi::um::winuser::WM_HOTKEY;

lazy_static! {
    static ref ACTIONS_WNDCLASS: UserControlClass<HotkeySink> = {
        WindowClass::build()
            .class_name("usewin::Actions::HotkeySink")
            .module(app_instance())
            .register_user_control::<HotkeySink>()
            .unwrap()
    };
}

struct HotkeySinkInner {
    functions: HashMap<i32, Rc<Fn()>>,
    last_id: i32,
    actions: HashMap<Cow<'static, str>, (HotKey, Action)>
}

struct HotkeySink(RefCell<HotkeySinkInner>);

impl HotkeySinkInner {
    pub fn set_action(&mut self, hwnd: HWND, action: Action) -> Result<()> {
        if self.actions.contains_key(&action.id) {
            self.remove_action(&action.id);
        }

        self.last_id += 1;
        if self.last_id > 0xBFFF {
            return Err(io::Error::new(io::ErrorKind::Other, "no hotkey identifiers left"))
        }

        let hotkey = HotKey::new(action.modifiers as isize, action.vk as i32, &hwnd, self.last_id)?;
        let func = action.func.clone();
        self.actions.insert(action.id.clone(), (hotkey, action));
        self.functions.insert(self.last_id, func);

        Ok(())
    }

    pub fn remove_action(&mut self, id: &str) {
        if let Some((hotkey, action)) = self.actions.remove(id) {
            self.functions.remove(&hotkey.id());
        }
    }

    pub fn clear_actions(&mut self) {
        self.functions.clear();
        self.actions.clear();
    }
}

pub struct Action {
    pub id: Cow<'static, str>,
    pub modifiers: u32, // TODO: change to Vec<Key> or KeyCombination
    pub vk: u32,
    pub func: Rc<Fn()>
}

pub struct Actions {
    window: UserControl<HotkeySink>
}

impl Actions {

    pub fn new() -> Actions {
        Actions {
            window: ACTIONS_WNDCLASS.build_window()
                .module(app_instance())
                .message_only()
                .style(WS_POPUP)
                .ex_style(WS_EX_TOOLWINDOW)
                .create(HotkeySink(RefCell::new(HotkeySinkInner {
                    functions: HashMap::new(),
                    last_id: -1,
                    actions: HashMap::new()
                })))
                .unwrap(),
        }
    }

    pub fn set_action(&mut self, action: Action) {
        let hwnd = self.window.as_hwnd();
        self.window.get().0.borrow_mut().set_action(hwnd, action);  // TODO: report error
    }

    pub fn remove_action(&mut self, id: &str) {
        self.window.get().0.borrow_mut().remove_action(id)
    }

}

impl UsrCtrl for HotkeySink {
    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        if msg != WM_HOTKEY {
            if msg == WM_DESTROY {
                self.0.borrow_mut().clear_actions();
            }

            return hwnd.default_proc(msg, w, l);
        }

        let id = w.into_raw() as i32;
        if let Some(func) = self.0.borrow().functions.get(&id) {
            func();
        }

        LResult::from(0)
    }
}