use std::borrow::Cow;

use crate::{result, Result, Wstr};
use winapi::um::shellapi::{
    Shell_NotifyIconW, NIF_ICON, NIF_MESSAGE, NIF_SHOWTIP, NIF_TIP, NIM_ADD, NIM_DELETE,
    NIM_MODIFY, NIM_SETVERSION, NOTIFYICONDATAW, NOTIFYICON_VERSION_4,
};

use crate::extra::icon::Icon;
use crate::sys::Window;

pub struct TrayIcon {
    hwnd: Window,
    id: u32,
}

impl TrayIcon {
    pub fn build(hwnd: Window, id: u32) -> TrayIconBuilder {
        TrayIconBuilder::new(hwnd, id)
    }

    pub fn id(&self) -> u32 {
        self.id
    }

    pub fn change(&self) -> TrayIconBuilder {
        TrayIconBuilder::new(self.hwnd, self.id)
    }
}

impl Drop for TrayIcon {
    fn drop(&mut self) {
        self.change().delete().unwrap_or_default();
    }
}

pub struct TrayIconBuilder {
    inner: NOTIFYICONDATAW,
}

impl TrayIconBuilder {
    fn new(hwnd: Window, id: u32) -> Self {
        unsafe {
            let mut inner: NOTIFYICONDATAW = std::mem::zeroed();
            inner.cbSize = std::mem::size_of::<NOTIFYICONDATAW>() as u32;
            inner.uID = id;
            inner.hWnd = hwnd.as_hwnd();
            inner.uFlags = 0;
            TrayIconBuilder { inner }
        }
    }

    pub fn tool_tip<'t, T: Into<Cow<'t, Wstr>>>(&mut self, value: T) -> &mut Self {
        self._tool_tip(&value.into());
        self
    }

    fn _tool_tip(&mut self, value: &Wstr) {
        self.inner.uFlags |= NIF_TIP | NIF_SHOWTIP;
        value.copy_to(&mut self.inner.szTip);
    }

    pub fn callback_message(&mut self, msg: u32) -> &mut Self {
        self.inner.uFlags |= NIF_MESSAGE;
        self.inner.uCallbackMessage = msg;
        self
    }

    pub fn icon(&mut self, icon: Icon) -> &mut Self {
        self.inner.uFlags |= NIF_ICON;
        self.inner.hIcon = icon.as_raw();
        self
    }

    pub fn add(&mut self) -> Result<TrayIcon> {
        unsafe {
            result(Shell_NotifyIconW(NIM_ADD, &mut self.inner))?;
            *self.inner.u.uVersion_mut() = NOTIFYICON_VERSION_4;
            result(Shell_NotifyIconW(NIM_SETVERSION, &mut self.inner))?;
        }
        Ok(TrayIcon {
            hwnd: Window::from(self.inner.hWnd),
            id: self.inner.uID,
        })
    }

    pub fn modify(&mut self) -> Result<()> {
        unsafe {
            result(Shell_NotifyIconW(NIM_MODIFY, &mut self.inner))?;
        }
        Ok(())
    }

    pub fn delete(&mut self) -> Result<()> {
        unsafe {
            result(Shell_NotifyIconW(NIM_DELETE, &mut self.inner))?;
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_builder_new() {}
}
