extern crate lightports;
extern crate lightports_gui;
extern crate winapi;

use std::ptr;

use lightports::debug::output_debug_string;
use lightports_gui::{
    sys::{Window, WindowClass}
};
use lightports_gui::sys::dispatch_messages;
use lightports_gui::sys::LParam;
use lightports_gui::sys::{WParam, LResult};
use lightports_gui::user_control::UserControlClassBuilderExt;
use lightports_gui::usr_ctrl::UsrCtrl;
use winapi::{
    um::winuser::{GetMessageW, MSG, WS_EX_APPWINDOW, WS_OVERLAPPEDWINDOW, WM_CREATE, WM_CLOSE, WM_USER, WM_MOUSEFIRST}
};
use winapi::um::libloaderapi::GetModuleHandleW;
use winapi::um::winuser::SW_SHOW;
use lightports_gui::shell::TrayIcon;
use lightports_gui::sys::post_quit_message;
use std::cell::RefCell;

const MSG_TRAYICON: u32 = WM_USER + 0x27;


struct MyControl {
    tray_icon: RefCell<TrayIcon>
}

impl MyControl {
    pub fn new() -> Self {
        MyControl {
            tray_icon: RefCell::new(TrayIcon::new(42, MSG_TRAYICON))
        }
    }
}

impl UsrCtrl for MyControl {
    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        match msg {
            WM_CREATE => {
                self.tray_icon.borrow_mut().attach(hwnd.as_hwnd())
            },
            WM_CLOSE => {
                post_quit_message(0);
            },

            MSG_TRAYICON if l.high_word() == 42 => {
                println!("msg: {:x} x: {} y: {}", l.low_word(), w.get_x(), w.get_y());
            },

            _ => ()
        }

        hwnd.default_proc(msg, w, l)
    }
}


fn main() {
    println!("Hello, world!");
    output_debug_string("Hello, windows!");

    let module = unsafe { GetModuleHandleW(ptr::null_mut()) };

    let win_class = WindowClass::build()
        .class_name("UserControl::test_message")
        .module(module)
        .register_user_control::<MyControl>()
        .unwrap();

    let mut window = win_class
        .build_window()
        .module(module)
        .create(MyControl::new())
        .unwrap();

    window.as_hwnd().show(SW_SHOW);

    dispatch_messages();
}
