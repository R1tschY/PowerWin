use crate::sys::{Window, WParam, LParam, LResult, WindowClassStyle};

pub trait UsrCtrl {
    fn class_style() -> WindowClassStyle {
        WindowClassStyle::default()
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        hwnd.default_proc(msg, w, l)
    }
}

