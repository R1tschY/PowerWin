use std::cell::Cell;
use std::ffi;
use std::ffi::OsStr;
use std::marker::PhantomData;
use std::mem;
use std::panic;
use std::ptr;

use lightports::Result;
use log::error;
use winapi::shared::minwindef::{HINSTANCE, LPARAM, LRESULT, UINT, WPARAM};
use winapi::shared::windef::{HMENU, HWND};
use winapi::um::winuser::{CREATESTRUCTW, GWLP_USERDATA, WM_CREATE, WM_NCDESTROY};
use winapi::um::winuser::HWND_MESSAGE;

use crate::sys::{AsHwnd, IsA, LParam, LResult, Window, WindowBuilder, WindowClass, WindowClassBuilder, WParam};
use crate::sys::WindowFunctions;
use crate::usr_ctrl::UsrCtrl;

struct CreateData<'t, T: UsrCtrl> {
    param: &'t T::CreateParam,
    target: *mut UserControlData<T>,
}

struct UserControlData<T: UsrCtrl> {
    hwnd: Cell<Window>,
    ctrl: Option<T>,
}

impl<T: UsrCtrl> UserControlData<T> {
    pub fn as_ptr(&self) -> *const ffi::c_void {
        self as *const UserControlData<T> as *const ffi::c_void
    }

    pub fn message(&self, hwnd: Window, msg: UINT, w: WPARAM, l: LPARAM) -> LResult {
        self.ctrl.as_ref().unwrap().message(hwnd, msg, WParam::from(w), LParam::from(l))
    }
}

pub struct UserControlClass<T: UsrCtrl>(WindowClass, PhantomData<T>);
unsafe impl<T: UsrCtrl> Send for UserControlClass<T> {}
unsafe impl<T: UsrCtrl> Sync for UserControlClass<T> {}

impl<T: UsrCtrl> UserControlClass<T> {
    pub fn build_window(&self) -> UserControlBuilder<T> {
        let mut builder = Window::build();
        builder.class(&self.0);
        UserControlBuilder { inner: builder, _type: PhantomData }
    }
}

pub struct UserControl<T: UsrCtrl>(Box<UserControlData<T>>);

impl<T: UsrCtrl> UserControl<T> {
    pub fn as_hwnd(&self) -> HWND {
        self.0.hwnd.get().as_hwnd()
    }

    pub fn get(&self) -> &T {
        self.0.ctrl.as_ref().unwrap()
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

impl<T: UsrCtrl> From<UserControl<T>> for Window {
    fn from(f: UserControl<T>) -> Self {
        Window::from(f.0.hwnd.get())
    }
}

impl<T: UsrCtrl> AsHwnd for UserControl<T> {
    fn as_hwnd(&self) -> HWND {
        self.0.hwnd.get().as_hwnd()
    }
}

impl<T: UsrCtrl + 'static> IsA<Window> for UserControl<T> { }

fn prepare_hwnd<T: UsrCtrl>(hwnd: Window, create_struct: *const CREATESTRUCTW) -> *const UserControlData<T> {
    unsafe {
        let p: *mut CreateData<T> = mem::transmute((*create_struct).lpCreateParams);
        assert_ne!(p, ptr::null_mut());

        let target = (*p).target;
        (*target).ctrl = Some(T::create(hwnd, &(*p).param));
        hwnd.set_attribute(GWLP_USERDATA, mem::transmute(target));
        (*target).hwnd.replace(hwnd);
        target
    }
}

fn user_control_proc<T: UsrCtrl>(
    hwnd: Window, msg: UINT, w: WPARAM, l: LPARAM
) -> Result<LRESULT> {
    let mut p: *const UserControlData<T> = unsafe {
        mem::transmute(hwnd.get_attribute(GWLP_USERDATA)?) };
    if p == ptr::null_mut() {
        if msg != WM_CREATE {
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
    match user_control_proc::<T>(Window::from(hwnd), msg, w, l) {
        Ok(result) => result,
        Err(err) => {
            error!("unhandled error: {}", err);
            Window::from(hwnd).default_proc(msg, w, l).into_raw()
        }
    }
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

    /// sets parent to HWND_MESSAGE
    pub fn message_only(&mut self) -> &mut Self {
        self.inner.parent(HWND_MESSAGE);
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

    pub fn create(&mut self, param: &T::CreateParam) -> Result<UserControl<T>> {
        let mut data = Box::new(UserControlData {
            hwnd: Cell::new(Window::from(ptr::null_mut())),
            ctrl: None
        });
        let mut create_data = CreateData { param, target: &mut *data };
        self.inner.create_param(&mut create_data as *mut CreateData<T> as *mut ffi::c_void);
        self.inner.create()?;
        Ok(UserControl(data))
    }
}

#[cfg(test)]
mod tests {
    use winapi::um::libloaderapi::GetModuleHandleW;
    use winapi::um::winuser::{WM_CREATE, WM_USER};

    use crate::sys::send_message;

    use super::*;

    const WM_WPARAM: u32 = WM_USER;
    const WM_LPARAM: u32 = WM_USER + 1;

    struct MyControl {
        data: Cell<u32>
    }

    impl UsrCtrl for MyControl {
        type CreateParam = ();

        fn create(hwnd: Window, params: &()) -> Self {
            MyControl { data: Cell::new(0) }
        }

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
            .create(&())
            .unwrap();
        assert_eq!(window.get().data.get(), 42);

        send_message(window.as_hwnd(), WM_WPARAM, 5, 0);
        assert_eq!(window.get().data.get(), 5);

        send_message(window.as_hwnd(), WM_LPARAM, 0, 28);
        assert_eq!(window.get().data.get(), 28);
    }
}