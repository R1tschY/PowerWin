use std::borrow::Cow;
use std::cell::RefCell;
use std::collections::HashMap;
use std::io;

use lightports::app::app_instance;
use lightports::extra::hotkey::HotKey;
use lightports::sys::*;
use lightports::user_control::*;
use lightports::usr_ctrl::UsrCtrl;
use log::{error, info, warn};
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
    hwnd: HWND,
    last_id: i32,
    functions: HashMap<i32, Cow<'static, str>>,
    hotkeys: HashMap<Cow<'static, str>, HotKey>,
    actions: HashMap<Cow<'static, str>, Action>,
}

struct HotkeySink(RefCell<HotkeySinkInner>);

impl HotkeySinkInner {
    fn parse_key_combination_to_vk(input: &str) -> io::Result<Vec<Key>> {
        if let Some(combi) = parse_key_combination(input) {
            Ok(combi)
        } else {
            Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "invalid key combination",
            ))
        }
    }

    pub fn set_action(&mut self, input: &str, id: Cow<'static, str>, func: Option<Box<dyn Fn()>>) {
        let keys = Self::parse_key_combination(input);
        let action = Action { id, keys, func };

        if self.actions.contains_key(action.id()) {
            warn!("Action {} will be overwriten", action.id());
        }

        self.actions.insert(action.id.clone(), action);
    }

    fn parse_key_combination(input: &str) -> Vec<Key> {
        let keys = if !input.is_empty() {
            match Self::parse_key_combination_to_vk(input) {
                Ok(keys) => keys,
                Err(err) => {
                    error!("invalid key combination {}: {}", input, err);
                    vec![]
                }
            }
        } else {
            vec![]
        };
        keys
    }

    pub fn refresh_actions(&mut self) {
        for action in self.actions.values_mut() {
            if action.keys.is_empty() || action.func.is_none() {
                if let Some(hotkey) = self.hotkeys.remove(action.id()) {
                    self.functions.remove(&hotkey.id());
                }
                continue;
            }
            info!("Registering {} to {:?}", action.id(), action.keys());

            self.last_id += 1;
            if self.last_id > 0xBFFF {
                error!("no hotkey identifiers left");
                return;
            }

            if let Ok((modifiers, vk)) = to_vk(action.keys()) {
                let hotkey = HotKey::new(modifiers as isize, vk as i32, self.hwnd, self.last_id);
                if let Ok(hotkey) = hotkey {
                    self.hotkeys.insert(action.id.clone(), hotkey);
                    self.functions.insert(self.last_id, action.id.clone());
                } else {
                    error!("failed to register hotkey: {:?}", hotkey.err().unwrap());
                }
            } else {
                error!(
                    "failed to translate key combination {:?} to virtual key and modifier",
                    action.keys
                );
            }
        }
    }

    pub fn remove_action(&mut self, id: &str) {
        if let Some(_action) = self.actions.remove(id) {
            if let Some(hotkey) = self.hotkeys.remove(id) {
                self.functions.remove(&hotkey.id());
            }
        }
    }

    pub fn remap(&mut self, id: &str, keys: &str) {
        if let Some(action) = self.actions.get_mut(id) {
            action.keys = Self::parse_key_combination(keys);
        } else {
            error!("Cannot remap non-existend action {} to {:?}", id, keys);
        }
    }

    pub fn clear_actions(&mut self) {
        self.functions.clear();
        self.actions.clear();
        self.hotkeys.clear();
    }

    pub fn dump_actions(&self) {
        for action in self.actions.values() {
            info!("Registered action {} to {:?}", action.id(), action.keys());
        }
    }
}

pub struct Action {
    id: Cow<'static, str>,
    keys: Vec<Key>,
    func: Option<Box<dyn Fn()>>,
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
        func: Option<Box<dyn Fn()>>,
    ) {
        self.window.get().0.borrow_mut().set_action(input, id, func);
    }

    pub fn set_doc_action<I: Into<Cow<'static, str>>>(&mut self, id: I, keys: &str) {
        self.set_action_internal(keys, id.into(), None);
    }

    pub fn set_action<I: Into<Cow<'static, str>>>(
        &mut self,
        id: I,
        keys: &str,
        f: impl Fn() + 'static,
    ) {
        self.set_action_internal(keys, id.into(), Some(Box::new(f)));
    }

    pub fn set_system_action<I: Into<Cow<'static, str>>, T: Fn() -> io::Result<()> + 'static>(
        &mut self,
        id: I,
        keys: &str,
        f: T,
    ) {
        let id = id.into();
        self.set_action(id.clone(), keys, move || match f() {
            Ok(_) => (),
            Err(err) => error!("action {} failed: {}", id, err),
        })
    }

    pub fn remove_action(&mut self, id: &str) {
        self.window.get().0.borrow_mut().remove_action(id)
    }

    pub fn refresh_actions(&mut self) {
        self.window.get().0.borrow_mut().refresh_actions();
    }

    pub fn dump_actions(&self) {
        self.window.get().0.borrow().dump_actions();
    }

    pub fn remap(&mut self, id: &str, keys: &str) {
        self.window.get().0.borrow_mut().remap(id, keys);
    }
}

impl UsrCtrl for HotkeySink {
    type CreateParam = ();

    fn create(hwnd: Window, _params: &()) -> Self {
        HotkeySink(RefCell::new(HotkeySinkInner {
            hwnd: hwnd.as_hwnd(),
            functions: HashMap::new(),
            last_id: -1,
            actions: HashMap::new(),
            hotkeys: HashMap::new(),
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
        let actions = self.0.borrow();
        if let Some(action_id) = actions.functions.get(&id) {
            if let Some(action) = actions.actions.get(action_id) {
                if let Some(func) = &action.func {
                    func();
                }
            }
        }

        0.into()
    }
}
