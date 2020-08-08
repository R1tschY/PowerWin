use std::cell::RefCell;
use std::collections::HashMap;
use std::io;
use std::rc::Rc;

use lazy_static::lazy_static;
use lightports::app::app_instance;
use lightports::extra::hotkey::HotKey;
use lightports::sys::*;
use lightports::user_control::UserControl;
use lightports::user_control::*;
use lightports::usr_ctrl::UsrCtrl;
use log::{debug, error, info};
use winapi::shared::windef::HWND;
use winapi::um::winuser::WM_DESTROY;
use winapi::um::winuser::WM_HOTKEY;

use crate::core::actions::{Action, KeyBindings};
use crate::key_combination::to_vk;

lazy_static! {
    static ref HOTKEY_WNDCLASS: UserControlClass<HotkeySink> = {
        WindowClass::build()
            .class_name("usewin::Actions::HotkeySink")
            .module(app_instance())
            .register_user_control::<HotkeySink>()
            .unwrap()
    };
}

pub trait HotKeyService {
    fn refresh(&self, bindings: &KeyBindings);
}

pub struct HotKeyServiceImpl {
    window: UserControl<HotkeySink>,
}

impl HotKeyServiceImpl {
    pub fn new() -> io::Result<Self> {
        Ok(Self {
            window: HOTKEY_WNDCLASS.build_window().message_only().create(&())?,
        })
    }

    fn get_inner(&self) -> &RefCell<HotkeySinkInner> {
        &self.window.get().0
    }
}

impl HotKeyService for HotKeyServiceImpl {
    fn refresh(&self, bindings: &KeyBindings) {
        self.get_inner().borrow_mut().refresh(bindings)
    }
}

struct HotkeySinkInner {
    hwnd: HWND,
    last_id: i32,
    bindings: HashMap<i32, Rc<Action>>,
    hotkeys: Vec<HotKey>,
}

struct HotkeySink(RefCell<HotkeySinkInner>);

impl HotkeySinkInner {
    pub fn refresh(&mut self, bindings: &KeyBindings) {
        debug!("Refresh hot key registration");
        self.cleanup();

        for (ks, action) in bindings.bindings().iter() {
            if ks.is_empty() || action.is_empty() {
                continue;
            }
            info!("Registering {} to `{}`", action.id(), ks);

            self.last_id += 1;
            if self.last_id > 0xBFFF {
                error!("No hotkey identifiers left");
                return;
            }

            if let Ok((modifiers, vk)) = to_vk(ks.keys()) {
                let hotkey = HotKey::new(modifiers as isize, vk as i32, self.hwnd, self.last_id);
                match hotkey {
                    Ok(hotkey) => {
                        self.hotkeys.push(hotkey);
                        self.bindings.insert(self.last_id, action.clone());
                    }
                    Err(err) => error!("Failed to register hotkey `{}`: {}", ks, err),
                }
            } else {
                error!(
                    "Failed to translate key combination `{}` to virtual key and modifier",
                    ks
                );
            }
        }
    }

    pub fn cleanup(&mut self) {
        self.bindings.clear();
        self.hotkeys.clear();
        self.last_id = -1;
    }
}

impl UsrCtrl for HotkeySink {
    type CreateParam = ();

    fn create(hwnd: Window, _params: &()) -> Self {
        HotkeySink(RefCell::new(HotkeySinkInner {
            hwnd: hwnd.as_hwnd(),
            last_id: -1,
            bindings: Default::default(),
            hotkeys: Default::default(),
        }))
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        if msg != WM_HOTKEY {
            if msg == WM_DESTROY {
                self.0.borrow_mut().cleanup();
            }

            return hwnd.default_proc(msg, w, l);
        }

        let id = w.as_raw() as i32;
        let data = self.0.borrow();
        if let Some(action) = data.bindings.get(&id) {
            action.trigger();
        } else {
            error!("No hotkey found for ID {}", id);
        }

        0.into()
    }
}
