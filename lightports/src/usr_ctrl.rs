use crate::sys::{LParam, LResult, WParam, Window, WindowClassStyle};

pub trait UsrCtrl {
    type CreateParam;

    fn create(hwnd: Window, param: &Self::CreateParam) -> Self;

    fn class_style() -> WindowClassStyle {
        WindowClassStyle::default()
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        hwnd.default_proc(msg, w, l)
    }
}
