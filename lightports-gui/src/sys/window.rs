use std::ffi::{c_void, OsStr};
use std::mem;
use std::ptr::null_mut;
use std::ptr;

use lightports::{clear_last_error, NonNull, Result, result, Wstr, WString};
use winapi::shared::minwindef::{UINT};
use winapi::shared::minwindef::HINSTANCE;
use winapi::shared::windef::{
    HMENU, HWND
};
use winapi::um::winuser::{DefWindowProcW, DestroyWindow, GetWindowLongPtrW, SetWindowLongPtrW, ShowWindow};
use winapi::um::winuser::CreateWindowExW;

use crate::sys::{WindowClass, WParam, LParam, LResult, AtomOrString};


bitflags! {
    #[derive(Default)]
    pub struct WindowExStyle: u32 {
        const ACCEPTFILES = 0x00000010;
        const APPWINDOW = 0x00040000;
        const CLIENTEDGE = 0x00000200;
        const COMPOSITED = 0x02000000;
        const CONTEXTHELP = 0x00000400;
        const CONTROLPARENT = 0x00010000;
        const DLGMODALFRAME = 0x00000001;
        const LAYERED = 0x0008000;
        const LAYOUTRTL = 0x00400000;
        const LEFT = 0x00000000;
        const LEFTSCROLLBAR = 0x00004000;
        const LTRREADING = 0x00000000;
        const MDICHILD = 0x00000040;
        const NOACTIVATE = 0x08000000;
        const NOINHERITLAYOUT = 0x00100000;
        const NOPARENTNOTIFY = 0x00000004;
        const NOREDIRECTIONBITMAP = 0x00200000;
        const RIGHT = 0x00001000;
        const RIGHTSCROLLBAR = 0x00000000;
        const RTLREADING = 0x00002000;
        const STATICEDGE = 0x00020000;
        const TOOLWINDOW = 0x00000080;
        const TOPMOST = 0x00000008;
        const TRANSPARENT = 0x00000020;
        const WINDOWEDGE = 0x00000100;

        const OVERLAPPEDWINDOW = (Self::WINDOWEDGE.bits | Self::CLIENTEDGE.bits);
        const PALETTEWINDOW = (Self::WINDOWEDGE.bits | Self::TOOLWINDOW.bits | Self::TOPMOST.bits);
    }
}

bitflags! {
    #[derive(Default)]
    pub struct WindowStyle: u32 {
        const BORDER = 0x00800000;
        const CAPTION = 0x00C00000;
        const CHILD = 0x40000000;
        const CHILDWINDOW = 0x40000000;
        const CLIPCHILDREN = 0x02000000;
        const CLIPSIBLINGS = 0x04000000;
        const DISABLED = 0x08000000;
        const DLGFRAME = 0x00400000;
        const GROUP = 0x00020000;
        const HSCROLL = 0x00100000;
        const ICONIC = 0x20000000;
        const MAXIMIZE = 0x01000000;
        const MAXIMIZEBOX = 0x00010000;
        const MINIMIZE = 0x20000000;
        const MINIMIZEBOX = 0x00020000;
        const OVERLAPPED = 0x00000000;
        const POPUP = 0x80000000;
        const SIZEBOX = 0x00040000;
        const SYSMENU = 0x00080000;
        const TABSTOP = 0x00010000;
        const THICKFRAME = 0x00040000;
        const TILED = 0x00000000;
        const VISIBLE = 0x10000000;
        const VSCROLL = 0x00200000;

        const OVERLAPPEDWINDOW = (
            Self::OVERLAPPED.bits | Self::CAPTION.bits | Self::SYSMENU.bits
            | Self::THICKFRAME.bits | Self::MINIMIZEBOX.bits | Self::MAXIMIZEBOX.bits);
        const POPUPWINDOW = (Self::POPUP.bits | Self::BORDER.bits | Self::SYSMENU.bits);
        const TILEDWINDOW = (
            Self::OVERLAPPED.bits | Self::CAPTION.bits | Self::SYSMENU.bits | Self::THICKFRAME.bits
            | Self::MINIMIZEBOX.bits | Self::MAXIMIZEBOX.bits);
    }
}


pub trait AsHwnd<'a> {
    fn as_hwnd(&self) -> HWND;
}

pub trait IsA<T>: for<'a> AsHwnd<'a> + 'static { }
impl<T> IsA<T> for T where T: for<'a> AsHwnd<'a> + 'static {}

pub trait WindowFunctions {
    fn destroy(&mut self) -> Result<()>;
    fn get_attribute(&self, index: i32) -> Result<isize>;
    fn set_attribute(&mut self, index: i32, data: isize);
    fn try_set_attribute(&mut self, index: i32, data: isize) -> Result<isize>;
}

impl<T: IsA<Window>> WindowFunctions for T {
    fn destroy(&mut self) -> Result<()> {
        unsafe { result(DestroyWindow(self.as_hwnd())).map(|_| ()) }
    }

    fn get_attribute(&self, index: i32) -> Result<isize> {
        unsafe {
            result(GetWindowLongPtrW(self.as_hwnd(), index))
        }
    }

    fn set_attribute(&mut self, index: i32, data: isize) {
        unsafe {
            SetWindowLongPtrW(self.as_hwnd(), index, data);
        }
    }

    fn try_set_attribute(&mut self, index: i32, data: isize) -> Result<isize> {
        clear_last_error();
        unsafe {
            result(SetWindowLongPtrW(self.as_hwnd(), index, mem::transmute(data)))
        }
    }
}


/// low level HWND abstraction
#[derive(Clone, Copy, Eq, PartialEq)]
pub struct Window(HWND);

impl Window {
    pub fn as_hwnd(&self) -> HWND { self.0 }

    pub fn build() -> WindowBuilder { WindowBuilder::new() }

    pub fn default_proc<W: Into<WParam>, L: Into<LParam>>(&self, msg: UINT, w: W, l: L) -> LResult {
        unsafe { LResult::from(DefWindowProcW(self.0, msg, w.into().into(), l.into().into())) }
    }

    pub fn show(&mut self, cmd: i32) -> bool {
        unsafe { ShowWindow(self.0, cmd) != 0 }
    }
}

impl<'a> AsHwnd<'a> for Window {
    fn as_hwnd(&self) -> HWND {
        assert!(self.0 != ptr::null_mut());
        self.0
    }
}

impl From<HWND> for Window {
    fn from(hwnd: HWND) -> Self {
        Window(hwnd)
    }
}

/// builder for a HWND
pub struct WindowBuilder {
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

impl WindowBuilder {
    fn new() -> Self {
        WindowBuilder {
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

    pub fn create(&self) -> Result<Window> {
        unsafe {
            result(Window(CreateWindowExW(
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

impl NonNull for Window {
    fn non_null(&self) -> bool {
        self.0 != null_mut()
    }
}