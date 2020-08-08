use crate::sys::{LParam, LResult, WParam, Window, WindowClassStyle};

/// implementation interface for user created Win32 controls
pub trait UsrCtrl {
    type CreateParam;

    /// called within CreateWindowEx to create user implementation
    fn create(hwnd: Window, param: &Self::CreateParam) -> Self;

    /// window class style used to create windows
    fn class_style() -> WindowClassStyle {
        WindowClassStyle::default()
    }

    /// message handler
    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        hwnd.default_proc(msg, w, l)
    }
}
