use lightports::extra::icon::Icon;
use lightports::extra::resources::ResourceSize;
use lightports::{
    shell::TrayIcon,
    sys::post_quit_message,
    sys::LParam,
    sys::Window,
    sys::{LResult, WParam},
    usr_ctrl::UsrCtrl,
};
use winapi::shared::minwindef::HINSTANCE;
use winapi::um::winuser::*;

use crate::resources::POWERWIN_SMALL_ICON;

const MSG_TRAYICON: u32 = WM_USER + 0x27;

pub struct TrayControl {
    _tray_icon: TrayIcon,
}

impl UsrCtrl for TrayControl {
    type CreateParam = HINSTANCE;

    fn create(hwnd: Window, hinst: &HINSTANCE) -> Self {
        let tray_icon = TrayIcon::build(hwnd, 42)
            .callback_message(MSG_TRAYICON)
            .icon(
                Icon::from_resource_shared(*hinst, POWERWIN_SMALL_ICON, ResourceSize::Small)
                    .expect("app icon could not be loaded"),
            )
            .build()
            .expect("creation of tray icon failed");
        Self {
            _tray_icon: tray_icon,
        }
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        match msg {
            WM_CLOSE => {
                post_quit_message(0);
            }
            WM_HOTKEY => {
                println!("WM_HOTKEY");
            }

            MSG_TRAYICON if l.high_word() as u32 == self._tray_icon.id() => {
                println!("msg: {:x} x: {} y: {}", l.low_word(), w.get_x(), w.get_y());
            }

            _ => (),
        }

        hwnd.default_proc(msg, w, l)
    }
}
