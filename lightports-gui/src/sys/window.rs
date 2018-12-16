use std::ffi::{c_void, OsStr};
use std::mem;
use std::ptr::null_mut;

use lightports::{clear_last_error, NonNull, Result, result, Wstr, WString};
use winapi::shared::minwindef::{LRESULT, UINT};
use winapi::shared::minwindef::HINSTANCE;
use winapi::shared::windef::{
    HMENU, HWND
};
use winapi::um::winuser::{DefWindowProcW, DestroyWindow, GetWindowLongPtrW, SetWindowLongPtrW, ShowWindow};
use winapi::um::winuser::CreateWindowExW;

use crate::sys::{WindowClass, WParam, LParam, AtomOrString};

/// low level HWND abstraction
#[derive(Clone, Copy, Eq, PartialEq)]
pub struct HWnd(HWND);

impl HWnd {
    pub fn build() -> HWndBuilder { HWndBuilder::new() }

    pub fn as_hwnd(&self) -> HWND { self.0 }

    pub fn default_proc<W: Into<WParam>, L: Into<LParam>>(&self, msg: UINT, w: W, l: L) -> LRESULT {
        unsafe { DefWindowProcW(self.0, msg, w.into().into(), l.into().into()) }
    }

    pub fn show(&mut self, cmd: i32) -> bool {
        unsafe { ShowWindow(self.0, cmd) != 0 }
    }

    pub fn destroy(&mut self) -> Result<()> {
        unsafe { result(DestroyWindow(self.0)).map(|_| ()) }
    }

    pub fn get_attribute(&self, index: i32) -> Result<isize> {
        unsafe {
            result(GetWindowLongPtrW(self.0, index))
        }
    }

    pub fn set_attribute(&mut self, index: i32, data: isize) {
        unsafe {
            SetWindowLongPtrW(self.0, index, data);
        }
    }

    pub fn try_set_attribute(&mut self, index: i32, data: isize) -> Result<isize> {
        clear_last_error();
        unsafe {
            result(SetWindowLongPtrW(self.0, index, mem::transmute(data)))
        }
    }
}

impl From<HWND> for HWnd {
    fn from(hwnd: HWND) -> Self {
        HWnd(hwnd)
    }
}

/// builder for a HWND
pub struct HWndBuilder {
    ex_style: u32,
    class: AtomOrString,
    window_name: WString,
    style: u32,
    x: i32,
    y: i32,
    width: i32,
    height: i32,
    parent: HWND,
    menu: HMENU,
    module: HINSTANCE,
    create_param: *mut c_void
}

impl HWndBuilder {
    fn new() -> Self {
        HWndBuilder {
            ex_style: 0,
            class: AtomOrString::Str(WString::new()),
            window_name: WString::new(),
            style: 0,
            x: 0,
            y: 0,
            width: 0,
            height: 0,
            parent: null_mut(),
            menu: null_mut(),
            module: null_mut(),
            create_param: null_mut()
        }
    }

    pub fn ex_style(&mut self, value: u32) -> &mut Self {
        self.ex_style |= value;
        self
    }

    pub fn style(&mut self, value: u32) -> &mut Self {
        self.style |= value;
        self
    }

    pub fn class(&mut self, class: &WindowClass) -> &mut Self {
        self.class = AtomOrString::Atom(class.as_atom());
        self
    }

    pub fn class_name<T: AsRef<OsStr>>(&mut self, name: T) -> &mut Self {
        self.class = AtomOrString::Str(WString::from_str(name));
        self
    }

    pub fn window_name<T: AsRef<OsStr>>(&mut self, title: T) -> &mut Self {
        self.window_name = WString::from_str(title); // TODO: into
        self
    }

    pub fn pos(&mut self, x: i32, y: i32) -> &mut Self {
        self.x = x;
        self.y = y;
        self
    }

    pub fn size(&mut self, width: i32, height: i32) -> &mut Self {
        self.width = width;
        self.height = height;
        self
    }

    pub fn parent(&mut self, value: HWND) -> &mut Self {
        self.parent = value;
        self
    }

    pub fn menu(&mut self, value: HMENU) -> &mut Self {
        self.menu = value;
        self
    }

    pub fn module(&mut self, value: HINSTANCE) -> &mut Self {
        self.module = value;
        self
    }

    pub fn create_param(&mut self, value: *mut c_void) -> &mut Self {
        self.create_param = value;
        self
    }

    pub fn create(&self) -> Result<HWnd> {
        unsafe {
            result(HWnd(CreateWindowExW(
                self.ex_style,
                self.class.as_ptr(),
                self.window_name.as_ptr(),
                self.style,
                self.x, self.y,
                self.width, self.height,
                self.parent,
                self.menu,
                self.module,
                self.create_param
            )))
        }
    }
}

impl NonNull for HWnd {
    fn non_null(&self) -> bool {
        self.0 != null_mut()
    }
}