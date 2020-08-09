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
use lightports::extra::cursor::Cursor;
use lightports::extra::menu::{Menu, MenuFlags, MenuFunctions, MenuItem, TrackFlags};

const MSG_TRAYICON: u32 = WM_USER + 0x27;

const INFO_COMMAND: u16 = 1000;
// ----
const AUTOSTART_COMMAND: u16 = 1001;
// ---
const QUIT_COMMAND: u16 = 1002;

pub struct TrayControl {
    window: Window,
    _tray_icon: TrayIcon,
    popup_menu: Menu,
}

impl TrayControl {
    fn on_contextmenu(&self) -> LResult {
        self.popup_menu
            .track_popup_menu(
                TrackFlags::LEFTALIGN | TrackFlags::RIGHTBUTTON,
                Cursor::get_pos().expect("Failed to get cursor position"),
                self.window,
            )
            .unwrap();

        0.into()
    }
}

impl UsrCtrl for TrayControl {
    type CreateParam = HINSTANCE;

    fn create(window: Window, hinst: &HINSTANCE) -> Self {
        let tray_icon = TrayIcon::build(window, 42)
            .callback_message(MSG_TRAYICON)
            .icon(
                Icon::from_resource_shared(*hinst, POWERWIN_SMALL_ICON, ResourceSize::Small)
                    .expect("app icon could not be loaded"),
            )
            .build()
            .expect("creation of tray icon failed");

        let popup_menu = Menu::new_popup().expect("creation of popup menu failed");
        popup_menu
            .add(
                MenuItem::TextEntry("PowerWin 0.99".into(), INFO_COMMAND),
                MenuFlags::ENABLED,
            )
            .unwrap();
        popup_menu
            .add(
                MenuItem::TextEntry("Copyright © 2020 Richard Liebscher".into(), INFO_COMMAND),
                MenuFlags::ENABLED,
            )
            .unwrap();
        popup_menu
            .add(
                MenuItem::TextEntry("Licenced under GPL v3".into(), INFO_COMMAND),
                MenuFlags::ENABLED,
            )
            .unwrap();
        popup_menu
            .add(MenuItem::Seperator, MenuFlags::ENABLED)
            .unwrap();
        popup_menu
            .add(
                MenuItem::TextEntry("Start with system".into(), AUTOSTART_COMMAND),
                MenuFlags::ENABLED,
            )
            .unwrap();
        popup_menu
            .add(MenuItem::Seperator, MenuFlags::ENABLED)
            .unwrap();
        popup_menu
            .add(
                MenuItem::TextEntry("Quit".into(), QUIT_COMMAND),
                MenuFlags::ENABLED,
            )
            .unwrap();

        Self {
            window,
            _tray_icon: tray_icon,
            popup_menu,
        }
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        match msg {
            WM_CLOSE => {
                post_quit_message(0);
            }
            WM_COMMAND => match w.low_word() {
                INFO_COMMAND => {}
                AUTOSTART_COMMAND => {}
                QUIT_COMMAND => post_quit_message(0),
                _ => {}
            },
            MSG_TRAYICON => {
                return self._tray_icon.message(w, l, |msg, _pt| match msg {
                    WM_CONTEXTMENU | WM_RBUTTONDOWN => self.on_contextmenu(),
                    _ => 0.into(),
                });
            }
            _ => (),
        }

        hwnd.default_proc(msg, w, l)
    }
}
