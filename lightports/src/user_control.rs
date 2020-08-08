use std::cell::Cell;
use std::ffi;
use std::ffi::OsStr;
use std::io;
use std::marker::PhantomData;
use std::mem;
use std::panic;
use std::ptr;

use crate::{clear_last_error, Result};
use log::{error, log_enabled, trace, Level::Trace};
use winapi::shared::minwindef::{HINSTANCE, LPARAM, LRESULT, UINT, WPARAM};
use winapi::shared::windef::{HMENU, HWND};
use winapi::um::winuser::HWND_MESSAGE;
use winapi::um::winuser::{CREATESTRUCTW, GWLP_USERDATA, WM_CREATE, WM_NCDESTROY};

use crate::messages::get_message_name;
use crate::sys::WindowFunctions;
use crate::sys::{
    AsHwnd, IsA, LParam, LResult, WParam, Window, WindowBuilder, WindowClass, WindowClassBuilder,
};
use crate::usr_ctrl::UsrCtrl;

struct CreateData<'t, T: UsrCtrl> {
    param: &'t T::CreateParam,
    target: &'t mut Option<Box<UserControlData<T>>>,
}

struct UserControlData<T: UsrCtrl> {
    hwnd: Cell<Window>,
    ctrl: T,
}

impl<T: UsrCtrl> UserControlData<T> {
    pub fn message(&self, hwnd: Window, msg: UINT, w: WPARAM, l: LPARAM) -> LResult {
        self.ctrl
            .message(hwnd, msg, WParam::from(w), LParam::from(l))
    }
}

/// Window class for user implementated Win32 controls
pub struct UserControlClass<T: UsrCtrl>(WindowClass, PhantomData<T>);
unsafe impl<T: UsrCtrl> Send for UserControlClass<T> {}
unsafe impl<T: UsrCtrl> Sync for UserControlClass<T> {}

impl<T: UsrCtrl> UserControlClass<T> {
    pub fn build_window(&self) -> UserControlBuilder<T> {
        let mut builder = Window::build();
        builder.class(&self.0);
        UserControlBuilder {
            inner: builder,
            _type: PhantomData,
        }
    }
}

/// user implementated Win32 control
pub struct UserControl<T: UsrCtrl>(Box<UserControlData<T>>);

impl<T: UsrCtrl> UserControl<T> {
    pub fn as_hwnd(&self) -> HWND {
        self.0.hwnd.get().as_hwnd()
    }

    pub fn get(&self) -> &T {
        &self.0.ctrl
    }
}

/// Extension methods for [WindowClassBuilder]
pub trait UserControlClassBuilderExt {
    /// wnd_proc of builder is ignored
    fn register_user_control<T: UsrCtrl>(&self) -> Result<UserControlClass<T>>;
}

impl UserControlClassBuilderExt for WindowClassBuilder {
    fn register_user_control<T: UsrCtrl>(&self) -> Result<UserControlClass<T>> {
        let class = self
            .clone()
            .wnd_proc(Some(unsafe_user_control_proc::<T>))
            .register()?;
        Ok(UserControlClass(class, PhantomData))
    }
}

pub struct UserControlBuilder<T: UsrCtrl> {
    inner: WindowBuilder,
    _type: PhantomData<T>,
}

fn _drop_data(window: Window) {
    if !window.is_null() {
        window.destroy().expect("Failed to destroy window"); // TODO: only log
    }
}

impl<T: UsrCtrl> Drop for UserControlData<T> {
    fn drop(&mut self) {
        _drop_data(self.hwnd.get())
    }
}

impl<T: UsrCtrl> From<UserControl<T>> for Window {
    fn from(f: UserControl<T>) -> Self {
        f.0.hwnd.get()
    }
}

impl<T: UsrCtrl> AsHwnd for UserControl<T> {
    fn as_hwnd(&self) -> HWND {
        self.0.hwnd.get().as_hwnd()
    }
}

impl<T: UsrCtrl + 'static> IsA<Window> for UserControl<T> {}

fn create_userdata<T: UsrCtrl>(
    hwnd: Window,
    create_struct: *const CREATESTRUCTW,
) -> *const UserControlData<T> {
    unsafe {
        let p = (*create_struct).lpCreateParams as *mut CreateData<T>;
        assert!(!p.is_null());

        let data = Box::new(UserControlData {
            hwnd: Cell::new(hwnd),
            ctrl: T::create(hwnd, &(*p).param),
        });
        let data_ptr = data.as_ref() as *const UserControlData<T>;

        *(*p).target = Some(data);
        // TODO: move out to reduce code bloat
        match hwnd.set_attribute(GWLP_USERDATA, mem::transmute(data_ptr)) {
            Ok(_) => {}
            Err(err) => {
                error!("Failed to set GWLP_USERDATA on {:?}: {:?}", data_ptr, err);
                clear_last_error();
            }
        }
        data_ptr
    }
}

fn user_control_proc<T: UsrCtrl>(hwnd: Window, msg: UINT, w: WPARAM, l: LPARAM) -> LResult {
    // TODO: move out to reduce code bloat
    let mut p: *const UserControlData<T> = match hwnd.get_attribute(GWLP_USERDATA) {
        Ok(attr) => unsafe { mem::transmute(attr) },
        Err(err) => {
            error!("Error getting window user data attribute: {}", err);
            clear_last_error();
            return hwnd.default_proc(msg, w, l);
        }
    };

    if p.is_null() {
        if msg != WM_CREATE {
            // ignore everything before WM_CREATE
            return hwnd.default_proc(msg, w, l);
        }
        p = create_userdata(hwnd, unsafe { mem::transmute(l) });
    }

    let result = UserControlData::message(unsafe { &*p }, hwnd, msg, w, l);

    // TODO: move out to reduce code bloat
    if msg == WM_NCDESTROY {
        // TODO destroy user data
        unsafe { &*p }.hwnd.replace(Window::from(ptr::null_mut()));
    }

    if log_enabled!(Trace) {
        trace!(
            "Handling message id {} with {:?}",
            get_message_name(msg),
            result
        );
        clear_last_error();
    }
    result
}

unsafe extern "system" fn unsafe_user_control_proc<T: UsrCtrl>(
    hwnd: HWND,
    msg: UINT,
    w: WPARAM,
    l: LPARAM,
) -> LRESULT {
    user_control_proc::<T>(Window::from(hwnd), msg, w, l).as_raw()
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
        let mut maybe_result = None;
        let mut create_data = CreateData {
            param,
            target: &mut maybe_result,
        };
        self.inner
            .create_param(&mut create_data as *mut CreateData<T> as *mut ffi::c_void);
        let window = self.inner.create().expect("Failed to create window");
        // TODO: move out to reduce code bloat
        if window.is_null() {
            Err(io::Error::new(
                io::ErrorKind::Interrupted,
                "window creation aborted",
            ))
        } else if let Some(result) = maybe_result {
            Ok(UserControl(result))
        } else {
            unreachable!("windows user data was not created")
        }
    }
}

#[cfg(test)]
mod tests {
    use winapi::um::libloaderapi::GetModuleHandleW;
    use winapi::um::winuser::{WM_CREATE, WM_USER};

    use crate::sys::send_message;

    use super::*;
    use crate::extra::module::application_module;

    const WM_WPARAM: u32 = WM_USER;
    const WM_LPARAM: u32 = WM_USER + 1;

    struct MyControl {
        data: Cell<u32>,
    }

    impl UsrCtrl for MyControl {
        type CreateParam = ();

        fn create(hwnd: Window, params: &()) -> Self {
            MyControl { data: Cell::new(0) }
        }

        fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
            match msg {
                WM_CREATE => {
                    self.data.replace(42);
                }
                WM_WPARAM => {
                    self.data.replace(w.as_raw() as u32);
                }
                WM_LPARAM => {
                    self.data.replace(l.as_raw() as u32);
                }
                _ => (),
            };
            hwnd.default_proc(msg, w, l)
        }
    }

    #[test]
    fn test_message() {
        let module = application_module();

        let win_class = WindowClass::build()
            .class_name("UserControl::test_message")
            .module(module)
            .register_user_control::<MyControl>()
            .unwrap();

        let mut window = win_class.build_window().module(module).create(&()).unwrap();
        assert_eq!(window.get().data.get(), 42);

        send_message(window.as_hwnd(), WM_WPARAM, 5, 0);
        assert_eq!(window.get().data.get(), 5);

        send_message(window.as_hwnd(), WM_LPARAM, 0, 28);
        assert_eq!(window.get().data.get(), 28);
    }

    static mut HAS_CALLED_DROPPED: bool = false;

    #[test]
    fn test_drop() {
        struct MyControl;

        impl UsrCtrl for MyControl {
            type CreateParam = ();

            fn create(hwnd: Window, params: &()) -> Self {
                MyControl
            }
        }

        impl Drop for MyControl {
            fn drop(&mut self) {
                unsafe {
                    HAS_CALLED_DROPPED = true;
                }
            }
        }

        unsafe {
            HAS_CALLED_DROPPED = false;
        }

        let module = application_module();

        let win_class = WindowClass::build()
            .class_name("UserControl::test_message")
            .module(module)
            .register_user_control::<MyControl>()
            .unwrap();

        let mut window = win_class.build_window().module(module).create(&()).unwrap();

        assert_eq!(unsafe { HAS_CALLED_DROPPED }, false);
        drop(window);
        assert_eq!(unsafe { HAS_CALLED_DROPPED }, true);
    }
}
