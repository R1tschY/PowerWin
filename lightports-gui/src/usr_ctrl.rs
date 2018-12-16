use crate::sys::{HWnd, WParam, LParam, WindowClassStyle};

pub trait UsrCtrl {
    fn class_style() -> WindowClassStyle {
        WindowClassStyle::default()
    }

    fn message(&mut self, hwnd: HWnd, msg: u32, w: WParam, l: LParam) -> isize {
        hwnd.default_proc(msg, w, l)
    }
}

