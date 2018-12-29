use std::ffi::OsStr;
use std::mem;
use std::ptr;
use std::ffi;
use std::marker::PhantomData;
use std::panic;

use winapi::shared::minwindef::{LPARAM, LRESULT, UINT, WPARAM, HINSTANCE};
use winapi::shared::windef::{HWND, HMENU};
use winapi::um::winuser::{CREATESTRUCTW, GWLP_USERDATA, WM_NCCREATE, WM_NCDESTROY};
use lightports::Result;
use std::cell::Cell;

use crate::sys::{Window, WindowBuilder, AsHwnd, IsA, LParam, WParam, LResult, WindowClass, WindowClassBuilder};
use crate::usr_ctrl::UsrCtrl;
use crate::sys::WindowFunctions;

struct UserControlData<T: UsrCtrl> {
    hwnd: Cell<Window>,
    ctrl: T,
}

impl<T: UsrCtrl> UserControlData<T> {
    pub fn as_ptr(&self) -> *const ffi::c_void {
        self as *const UserControlData<T> as *const ffi::c_void
    }

    pub fn message(&self, hwnd: Window, msg: UINT, w: WPARAM, l: LPARAM) -> LResult {
        self.ctrl.message(hwnd, msg, WParam::from(w), LParam::from(l))
    }
}

pub struct UserControlClass<T: UsrCtrl>(WindowClass, PhantomData<T>);

impl<T: UsrCtrl> UserControlClass<T> {
    pub fn build_window(&self) -> UserControlBuilder<T> {
        let mut builder = Window::build();
        builder.class(&self.0);
        UserControlBuilder { inner: builder, _type: PhantomData }
    }
}

pub struct UserControl<T: UsrCtrl>(Box<UserControlData<T>>);

impl<T: UsrCtrl> UserControl<T> {
    pub fn as_hwnd(&mut self) -> Window {
        self.0.hwnd.get()
    }

    pub fn get(&self) -> &T {
        &self.0.ctrl
    }
}

pub trait UserControlClassBuilderExt {
    /// wnd_proc of builder is ignored
    fn register_user_control<T: UsrCtrl>(&self) -> Result<UserControlClass<T>>;
}

impl UserControlClassBuilderExt for WindowClassBuilder {
    fn register_user_control<T: UsrCtrl>(&self) -> Result<UserControlClass<T>> {
        let class = self.clone()
            .wnd_proc(Some(unsafe_user_control_proc::<T>))
            .register()?;
        Ok(UserControlClass(class, PhantomData))
    }
}

pub struct UserControlBuilder<T: UsrCtrl> {
    inner: WindowBuilder,
    _type: PhantomData<T>
}

impl<T: UsrCtrl> Drop for UserControlData<T> {
    fn drop(&mut self) {
        if self.hwnd.get().as_hwnd() != ptr::null_mut() {
            self.hwnd.get().destroy().unwrap();
        }
    }
}

impl<'a, T: UsrCtrl> AsHwnd<'a> for UserControl<T> {
    fn as_hwnd(&self) -> HWND {
        self.0.hwnd.get().as_hwnd()
    }
}

impl<T: UsrCtrl + 'static> IsA<Window> for UserControl<T> { }

// TODO: remove T
fn prepare_hwnd<T: UsrCtrl>(mut hwnd: Window, create_struct: *const CREATESTRUCTW) -> *const UserControlData<T> {
    unsafe {
        let p: *const UserControlData<T> = mem::transmute((*create_struct).lpCreateParams);
        assert_ne!(p, ptr::null_mut());

        hwnd.set_attribute(GWLP_USERDATA, mem::transmute(p));
        (*p).hwnd.replace(hwnd);
        p
    }
}

fn user_control_proc<T: UsrCtrl>(
    hwnd: Window, msg: UINT, w: WPARAM, l: LPARAM
) -> Result<LRESULT> {
    let mut p: *const UserControlData<T> = unsafe {
        mem::transmute(hwnd.get_attribute(GWLP_USERDATA)?) };
    if p == ptr::null_mut() {
        if msg != WM_NCCREATE {
            return Ok(hwnd.default_proc(msg, w, l).into_raw());
        }
        p = prepare_hwnd(hwnd, unsafe { mem::transmute(l) });
    }

    let result = UserControlData::message(unsafe { &*p }, hwnd, msg, w, l).into_raw();
    if msg == WM_NCDESTROY {
        unsafe { &*p }.hwnd.replace(Window::from(ptr::null_mut()));
    }
    Ok(result)
}

unsafe extern "system" fn unsafe_user_control_proc<T: UsrCtrl>(
    hwnd: HWND, msg: UINT, w: WPARAM, l: LPARAM
) -> LRESULT {
    user_control_proc::<T>(Window::from(hwnd), msg, w, l).unwrap()
}

impl<T: UsrCtrl> UserControlBuilder<T> {
    pub fn ex_style(&mut self, value: u32) -> &mut Self {
        self.inner.ex_style(value);
        self
    }

    pub fn style(&mut self, value: u32) -> &mut Self {
        self.inner.style(value);
        self
    }

    pub fn window_name<U: AsRef<OsStr>>(&mut self, title: U) -> &mut Self {
        self.inner.window_name(title);
        self
    }

    pub fn pos(&mut self, x: i32, y: i32) -> &mut Self {
        self.inner.pos(x, y);
        self
    }

    pub fn size(&mut self, width: i32, height: i32) -> &mut Self {
        self.inner.size(width, height);
        self
    }

    pub fn parent(&mut self, value: HWND) -> &mut Self {
        self.inner.parent(value);
        self
    }

    pub fn menu(&mut self, value: HMENU) -> &mut Self {
        self.inner.menu(value);
        self
    }

    pub fn module(&mut self, value: HINSTANCE) -> &mut Self {
        self.inner.module(value);
        self
    }

    pub fn create(&mut self, ctrl: T) -> Result<UserControl<T>> {
        let mut data = Box::new(UserControlData {
            hwnd: Cell::new(Window::from(ptr::null_mut())),
            ctrl
        });
        let cell_ptr: *mut UserControlData<T> =  &mut *data;
        self.inner.create_param(cell_ptr as *mut ffi::c_void);
        self.inner.create()?;
        Ok(UserControl(data))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use winapi::um::winuser::{WM_CREATE, WM_USER};
    use winapi::um::libloaderapi::GetModuleHandleW;
    use crate::sys::send_message;

    const WM_WPARAM: u32 = WM_USER;
    const WM_LPARAM: u32 = WM_USER + 1;

    struct MyControl {
        data: Cell<u32>
    }

    impl UsrCtrl for MyControl {
        fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
            match msg {
                WM_CREATE => { self.data.replace(42); }
                WM_WPARAM => { self.data.replace(w.into_raw() as u32); }
                WM_LPARAM => { self.data.replace(l.into_raw() as u32); }
                _ => ()
            };
            hwnd.default_proc(msg, w, l)
        }
    }

    #[test]
    fn test_message() {
        let module = unsafe { GetModuleHandleW(ptr::null_mut()) };

        let win_class = WindowClass::build()
            .class_name("UserControl::test_message")
            .module(module)
            .register_user_control::<MyControl>()
            .unwrap();

        let mut window = win_class
            .build_window()
            .module(module)
            .create(MyControl { data: Cell::new(0) })
            .unwrap();
        assert_eq!(window.get().data.get(), 42);

        send_message(window.as_hwnd(), WM_WPARAM, 5, 0);
        assert_eq!(window.get().data.get(), 5);

        send_message(window.as_hwnd(), WM_LPARAM, 0, 28);
        assert_eq!(window.get().data.get(), 28);
    }
}