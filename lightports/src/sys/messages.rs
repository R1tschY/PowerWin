use crate::sys::{LParam, WParam, Window};
use crate::Error;
use std::ptr;

use std::mem::MaybeUninit;
use winapi::shared::minwindef::LRESULT;
use winapi::um::winuser::PostQuitMessage;
use winapi::um::winuser::{DispatchMessageW, GetMessageW, SendMessageW, TranslateMessage, MSG};

pub fn dispatch_messages() -> i32 {
    unsafe {
        let mut msg: MaybeUninit<MSG> = MaybeUninit::uninit();
        loop {
            match GetMessageW(msg.as_mut_ptr(), ptr::null_mut(), 0, 0) {
                0 => return (*msg.as_ptr()).wParam as i32,
                -1 => panic!("error while message dispatch: {}", Error::last_os_error()),
                _ => {
                    TranslateMessage(msg.as_ptr());
                    DispatchMessageW(msg.as_ptr());
                }
            }
        }
    }
}

pub fn send_message<T: Into<Window>, W: Into<WParam>, L: Into<LParam>>(
    hwnd: T,
    msg: u32,
    w: W,
    l: L,
) -> LRESULT {
    unsafe { SendMessageW(hwnd.into().as_hwnd(), msg, w.into().into(), l.into().into()) }
}

pub fn post_quit_message(exit_code: i32) {
    unsafe {
        PostQuitMessage(exit_code);
    }
}
