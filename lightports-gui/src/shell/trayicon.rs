use winapi::um::shellapi::{
    NOTIFYICONDATAW,
    Shell_NotifyIconW,
    NIM_ADD, NIM_SETVERSION, NIM_DELETE, NIM_MODIFY,
    NIF_TIP, NIF_SHOWTIP,
    NOTIFYICON_VERSION_4,
};
use winapi::shared::windef::HWND;
use std::ptr::null_mut;
use lightports::{Wstr};

pub struct TrayIcon {
    inner: NOTIFYICONDATAW
}

impl TrayIcon {
    pub fn new(id: u32) -> TrayIcon {
        unsafe {
            let mut inner: NOTIFYICONDATAW = std::mem::zeroed();
            inner.cbSize = std::mem::size_of::<NOTIFYICONDATAW>() as u32;
            inner.uID = id;
            *inner.u.uVersion_mut() = NOTIFYICON_VERSION_4;

            TrayIcon { inner }
        }
    }

    pub fn attach(&mut self, hwnd: HWND) {
        unsafe {
            self.inner.uFlags = 0;
            self.inner.hWnd = hwnd;
            Shell_NotifyIconW(NIM_ADD, &mut self.inner);
            Shell_NotifyIconW(NIM_SETVERSION, &mut self.inner);
        }
    }

    pub fn detach(&mut self) {
        if self.inner.hWnd.is_null() {
            unsafe {
                Shell_NotifyIconW(NIM_DELETE, &mut self.inner);
            }
            self.inner.hWnd = null_mut();
            self.inner.uFlags = 0;
        }
    }

    // pub fn set_tool_tip<T: IntoWstr + 'static>(&mut self, value: T) {
    //     let wstr = value.into_wstr();
    //     self._set_tool_tip(&wstr);
    // }

    pub fn _set_tool_tip(&mut self, value: &Wstr) {
        self.inner.uFlags |= NIF_TIP | NIF_SHOWTIP;
        value.copy_to(&mut self.inner.szTip);
        self.update();
    }

    fn update(&mut self) {
        if self.inner.hWnd.is_null() {
            unsafe {
                Shell_NotifyIconW(NIM_MODIFY, &mut self.inner);
            }
            self.inner.uFlags = 0;
        }
    }
}

impl Drop for TrayIcon {
    fn drop(&mut self) {
        self.detach()
    }
}