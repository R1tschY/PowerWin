use std::borrow::Cow;
use std::cell::RefCell;
use std::collections::HashMap;
use std::io;
use std::rc::Rc;

use lightports::app::app_instance;
use lightports::extra::hotkey::HotKey;
use lightports::sys::*;
use lightports::user_control::*;
use lightports::usr_ctrl::UsrCtrl;
use lightports::Result;
use log::error;
use log::info;
use winapi::shared::windef::HWND;
use winapi::um::winuser::WM_DESTROY;
use winapi::um::winuser::WM_HOTKEY;
use winapi::um::winuser::WS_EX_TOOLWINDOW;
use winapi::um::winuser::WS_POPUP;

use lazy_static::lazy_static;

use crate::key_combination::{parse_key_combination, to_vk, Key};

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
    functions: HashMap<i32, Rc<dyn Fn()>>,
    last_id: i32,
    actions: HashMap<Cow<'static, str>, (Option<HotKey>, Action)>,
}

struct HotkeySink(RefCell<HotkeySinkInner>);

impl HotkeySinkInner {
    pub fn set_action(&mut self, hwnd: HWND, action: Action) -> Result<()> {
        if self.actions.contains_key(&action.id) {
            self.remove_action(&action.id);
        }

        self.last_id += 1;
        if self.last_id > 0xBFFF {
            return Err(io::Error::new(
                io::ErrorKind::Other,
                "no hotkey identifiers left",
            ));
        }

        if let Some(func) = action.func.clone() {
            let hotkey = HotKey::new(
                action.modifiers as isize,
                action.vk as i32,
                &hwnd,
                self.last_id,
            )?;
            self.actions
                .insert(action.id.clone(), (Some(hotkey), action));
            self.functions.insert(self.last_id, func);
        } else {
            self.actions.insert(action.id.clone(), (None, action));
        }

        Ok(())
    }

    pub fn remove_action(&mut self, id: &str) {
        if let Some((Some(hotkey), _action)) = self.actions.remove(id) {
            self.functions.remove(&hotkey.id());
        }
    }

    pub fn clear_actions(&mut self) {
        self.functions.clear();
        self.actions.clear();
    }

    pub fn dump_actions(&self) {
        for action in self.actions.values() {
            info!(
                "Registered action {} to {:?}",
                action.1.id(),
                action.1.keys()
            );
        }
    }
}

pub struct Action {
    id: Cow<'static, str>,
    modifiers: u32, // TODO: change to Vec<Key> or KeyCombination
    vk: u32,
    keys: Vec<Key>,
    func: Option<Rc<dyn Fn()>>,
}

impl Action {
    pub fn id(&self) -> &str {
        &self.id
    }

    pub fn keys(&self) -> &[Key] {
        &self.keys
    }
}

pub struct Actions {
    window: UserControl<HotkeySink>,
}

impl Actions {
    pub fn new() -> Actions {
        Actions {
            window: ACTIONS_WNDCLASS
                .build_window()
                .module(app_instance())
                .message_only()
                .style(WS_POPUP)
                .ex_style(WS_EX_TOOLWINDOW)
                .create(&())
                .unwrap(),
        }
    }

    fn set_action_internal(
        &mut self,
        input: &str,
        id: Cow<'static, str>,
        func: Option<Rc<dyn Fn()>>,
    ) {
        match Self::parse_key_combination_to_vk(input) {
            Ok((keys, (modifiers, vk))) => {
                let action = Action {
                    id,
                    modifiers,
                    vk,
                    keys,
                    func,
                };

                let hwnd = self.window.as_hwnd();
                let id = action.id.clone();
                if let Err(err) = self.window.get().0.borrow_mut().set_action(hwnd, action) {
                    error!("cannot register action {}: {}", id, err);
                }
            }
            Err(err) => error!("cannot register hotkey {}: {}", input, err),
        };
    }

    fn parse_key_combination_to_vk(input: &str) -> io::Result<(Vec<Key>, (u32, u32))> {
        if let Some(combi) = parse_key_combination(input) {
            to_vk(&combi).map(|vkey| (combi, vkey))
        } else {
            Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "invalid key combination",
            ))
        }
    }

    pub fn set_doc_action<I: Into<Cow<'static, str>>>(&mut self, id: I, keys: &str) {
        self.set_action_internal(keys, id.into(), None);
    }

    pub fn set_action<I: Into<Cow<'static, str>>>(&mut self, id: I, keys: &str, f: Rc<dyn Fn()>) {
        self.set_action_internal(keys, id.into(), Some(f));
    }

    pub fn set_system_action<I: Into<Cow<'static, str>>, T: Fn() -> io::Result<()> + 'static>(
        &mut self,
        id: I,
        keys: &str,
        f: T,
    ) {
        let id = id.into();
        self.set_action(
            id.clone(),
            keys,
            Rc::new(move || match f() {
                Ok(_) => (),
                Err(err) => error!("action {} failed: {}", id, err),
            }),
        )
    }

    pub fn remove_action(&mut self, id: &str) {
        self.window.get().0.borrow_mut().remove_action(id)
    }

    pub fn dump_actions(&self) {
        self.window.get().0.borrow().dump_actions();
    }
}

impl UsrCtrl for HotkeySink {
    type CreateParam = ();

    fn create(_hwnd: Window, _params: &()) -> Self {
        HotkeySink(RefCell::new(HotkeySinkInner {
            functions: HashMap::new(),
            last_id: -1,
            actions: HashMap::new(),
        }))
    }

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

        0.into()
    }
}
