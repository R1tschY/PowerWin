use winapi::shared::minwindef::{ATOM, HINSTANCE};
use winapi::shared::windef::{
    HICON, HCURSOR, HBRUSH
};
use std::ptr::null_mut;
use winapi::um::winuser::{RegisterClassExW, WNDCLASSEXW};
use std::io;
use std::ffi::OsStr;
use std::mem::size_of;
use winapi::um::winuser::WNDPROC;
use crate::{WString};

bitflags! {
    #[derive(Default)]
    pub struct WindowClassStyle: u32 {
        const VREDRAW = 0x0001;
        const HREDRAW = 0x0002;
        const DBLCLKS = 0x0008;
        const OWNDC = 0x0020;
        const CLASSDC = 0x0040;
        const PARENTDC = 0x0080;
        const NOCLOSE = 0x0200;
        const SAVEBITS = 0x0800;
        const BYTEALIGNCLIENT = 0x1000;
        const BYTEALIGNWINDOW = 0x2000;
        const GLOBALCLASS = 0x4000;
        const DROPSHADOW = 0x00020000;
    }
}


pub struct WindowClass(ATOM);

impl WindowClass  {
    pub fn build() -> WindowClassBuilder {
        WindowClassBuilder::new()
    }

    pub fn as_atom(&self) -> ATOM { self.0 }
}

#[derive(Clone)]
pub struct WindowClassBuilder {
    class_name: WString,
    style: u32,
    icon: HICON,
    small_icon: HICON,
    cursor: HCURSOR,
    background: HBRUSH,
    wnd_proc: WNDPROC,
    module: HINSTANCE,
    menu: WString,
}

impl WindowClassBuilder {
    pub fn new() -> WindowClassBuilder {
        WindowClassBuilder {
            class_name: WString::default(),
            style: 0,
            icon: null_mut(),
            small_icon: null_mut(),
            cursor: null_mut(),
            background: null_mut(),
            wnd_proc: None,
            module: null_mut(),
            menu: WString::default(),
        }
    }

    pub fn class_name<T: AsRef<OsStr>>(&mut self, value: T) -> &mut WindowClassBuilder {
        self.class_name = WString::from_str(value);
        self
    }

    pub fn style(&mut self, value: u32) -> &mut WindowClassBuilder {
        self.style = value;
        self
    }

    pub fn wnd_proc(&mut self, value: WNDPROC) -> &mut WindowClassBuilder {
        self.wnd_proc = value;
        self
    }

    pub fn module(&mut self, value: HINSTANCE) -> &mut WindowClassBuilder {
        self.module = value;
        self
    }

    pub fn register(&self) -> io::Result<WindowClass> {
        unsafe {
            let params = WNDCLASSEXW {
                cbSize: size_of::<WNDCLASSEXW>() as u32,
                style: self.style,
                lpfnWndProc: self.wnd_proc,
                cbClsExtra: 0,
                cbWndExtra: 0,
                hInstance: self.module,
                hIcon: self.icon,
                hCursor: self.cursor,
                hbrBackground: self.background,
                lpszMenuName: self.menu.as_ptr(),
                lpszClassName: self.class_name.as_ptr(),
                hIconSm: self.small_icon
            };

            Ok(WindowClass(RegisterClassExW(&params)))
        }
    }
}

