extern crate config;
extern crate lightports;
extern crate lightports_gui;
extern crate winapi;

use std::cell::RefCell;
use std::collections::HashMap;

use lightports::debug::output_debug_string;
use lightports_gui::{
    shell::TrayIcon,
    sys::{LResult, WParam},
    sys::{Window, WindowClass},
    sys::dispatch_messages,
    sys::LParam,
    sys::post_quit_message,
    sys::WindowFunctions,
    sys::WindowStyle,
    user_control::UserControlClassBuilderExt,
    usr_ctrl::UsrCtrl
};
use winapi::um::winuser::*;
use lightports_gui::app::app_instance;
use usewin::actions::Actions;
use usewin::actions::Action;
use std::rc::Rc;
use std::borrow::Cow;
use usewin::hotkeys::HotKeysModuleBuilder;
use usewin::module::{ModuleBuilder, ModuleContext};
use lightports_gui::shell::TrayIconBuilder;

const MSG_TRAYICON: u32 = WM_USER + 0x27;


struct MyControl {
    tray_icon: TrayIcon
}

impl UsrCtrl for MyControl {
    type CreateParam = ();

    fn create(hwnd: Window, params: &()) -> Self {
        let mut t = TrayIcon::build(hwnd, 42);
        let tray_icon = t
            .callback_message(MSG_TRAYICON)
            .add().expect("creation of tray icon failed");

        MyControl { tray_icon }
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        match msg {
            WM_CLOSE => {
                post_quit_message(0);
            },
            WM_HOTKEY => {
                println!("WM_HOTKEY");
            }

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

    let mut settings = config::Config::default();
    settings
        .merge(config::File::with_name("Settings")).unwrap();
    // println!("\n{:?} \n\n-----------",
    //          settings.try_into::<HashMap<String, HashMap<String, String>>>().unwrap());

    let module = app_instance();

    let win_class = WindowClass::build()
        .class_name("UserControl::test_message")
        .module(module)
        .register_user_control::<MyControl>()
        .unwrap();

    let window = win_class
        .build_window()
        .module(module)
        .style(WindowStyle::OVERLAPPEDWINDOW.bits())
        .size(300, 200)
        .pos(200, 200)
        .create(&())
        .unwrap();

//    let trayicon = TrayIcon::new(0, MSG_TRAYICON).attach(window.as_hwnd());

    let mut actions = Actions::new();
    let mut ctx = ModuleContext::new(&mut actions, &settings);
    let module = HotKeysModuleBuilder().build(&mut ctx);

    window.show(SW_SHOW);

    dispatch_messages();
}
