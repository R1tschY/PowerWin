use crate::def::point::Point;
use crate::WinResult;
use std::io;
use winapi::_core::mem::MaybeUninit;
use winapi::shared::windef::POINT;
use winapi::um::winuser::GetCursorPos;

pub struct Cursor;

impl Cursor {
    pub fn get_pos() -> io::Result<Point> {
        unsafe {
            let mut pos = MaybeUninit::<POINT>::uninit();
            GetCursorPos(pos.as_mut_ptr())
                .into_result()
                .map(|_| pos.assume_init().into())
        }
    }
}
