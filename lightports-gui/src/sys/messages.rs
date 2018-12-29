use std::ptr;
use std::mem;
use lightports::Error;
use crate::sys::{Window, WParam, LParam};

use winapi::shared::minwindef::LRESULT;
use winapi::um::winuser::{MSG, SendMessageW, GetMessageW, TranslateMessage, DispatchMessageW};
use winapi::um::winuser::PostQuitMessage;

pub fn dispatch_messages() -> i32 {
    unsafe {
        let mut msg: MSG = mem::zeroed();
        loop {
            match GetMessageW(&mut msg, ptr::null_mut(), 0, 0) {
                0 => return msg.wParam as i32,
                -1 => panic!("error while message dispatch: {}", Error::last_os_error()),
                _ => {
                    TranslateMessage(&msg);
                    DispatchMessageW(&msg);
                }
            }
        }
    }
}

pub fn send_message<T: Into<Window>, W: Into<WParam>, L: Into<LParam>>(
    hwnd: T, msg: u32, w: W, l: L
) -> LRESULT {
    unsafe {
        SendMessageW(hwnd.into().as_hwnd(), msg, w.into().into(), l.into().into())
    }
}

pub fn post_quit_message(exit_code: i32) {
    unsafe { PostQuitMessage(exit_code); }
}