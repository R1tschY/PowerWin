extern crate winapi;
extern crate lightports;
extern crate lightports_gui;

use lightports::debug::output_debug_string;
use lightports_gui::{
    sys::{HWnd, WindowClass}
};
use winapi::{
    shared::minwindef::{UINT, WPARAM, LPARAM, LRESULT},
    shared::windef::HWND,
    um::winuser::{GetMessageW, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW, MSG}
};
use std::ptr::null_mut;
use winapi::um::winuser::TranslateMessage;
use winapi::um::winuser::DispatchMessageW;
use winapi::um::libloaderapi::GetModuleHandleW;
use winapi::um::winuser::SW_SHOW;
use winapi::um::winuser::DefWindowProcW;


unsafe extern "system" fn window_sysproc(hwnd: HWND, msg: UINT, w: WPARAM, l: LPARAM) -> LRESULT {
    DefWindowProcW(hwnd, msg, w, l)
}

fn process_messages() -> i32 {
    unsafe {
        let mut msg: MSG = std::mem::zeroed();
        while GetMessageW(&mut msg, null_mut(), 0, 0) != 0 {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        msg.wParam as i32
    }
}


fn main() {
    println!("Hello, world!");
    output_debug_string("Hello, windows!");

    let win_class = WindowClass::build()
        .class_name("Winuse")
        .wnd_proc(Some(window_sysproc))
        .register()
        .unwrap();

    let mut window = HWnd::build()
        .class_name("Winuse")
        .window_name("⚡ Hello, world! ⚡")
        .pos(100, 100).size(300, 200)
        .style(WS_OVERLAPPEDWINDOW)
        .ex_style(WS_EX_APPWINDOW)
        .module(unsafe { GetModuleHandleW(null_mut()) })
        .create()
        .unwrap();

    window.show(SW_SHOW);

    process_messages();
}
