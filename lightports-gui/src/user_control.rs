use std::ffi::OsStr;
use std::mem;
use std::ptr;
use std::ffi;
use std::marker::PhantomData;
use std::cell::{Cell, RefCell, Ref, RefMut};
use std::panic;
use std::ops::Deref;

use winapi::shared::minwindef::{LPARAM, LRESULT, UINT, WPARAM, HINSTANCE};
use winapi::shared::windef::{HWND, HMENU};
use winapi::um::winuser::{CREATESTRUCTW, GWLP_USERDATA, WM_NCCREATE};
use winapi::um::libloaderapi::GetModuleHandleW;
use lightports::Result;

use crate::sys::{HWnd, HWndBuilder, LParam, WParam, WindowClass, WindowClassBuilder};
use crate::usr_ctrl::{UsrCtrl};

thread_local! {
    static PANIC: Cell<std::thread::Result<()>> = Cell::new(Ok(()));
}

struct UserControlData<T: UsrCtrl> {
    hwnd: HWnd,
    ctrl: T,
}

impl<T: UsrCtrl> UserControlData<T> {
    pub fn as_ptr(&mut self) -> *mut ffi::c_void {
        self as *mut UserControlData<T> as *mut ffi::c_void
    }

    pub fn message(&mut self, hwnd: HWnd, msg: UINT, w: WPARAM, l: LPARAM) -> LRESULT {
        debug_assert!(hwnd == self.hwnd);
        self.ctrl.message(hwnd, msg, WParam::from(w), LParam::from(l))
    }
}

pub struct UserControlClass<T: UsrCtrl>(WindowClass, PhantomData<T>);

impl<T: UsrCtrl> UserControlClass<T> {
    pub fn build_window(&self) -> UserControlBuilder<T> {
        let mut builder = HWnd::build();
        builder.class(&self.0);
        UserControlBuilder { inner: builder, _type: PhantomData }
    }
}

pub struct UserControl<T: UsrCtrl>(Box<RefCell<UserControlData<T>>>);

impl<T: UsrCtrl> UserControl<T> {
    pub fn as_hwnd(&mut self) -> HWnd {
        self.0.borrow().hwnd
    }

    pub fn borrow(&self) -> Ref<T> {
        Ref::map(self.0.borrow(), |t| &t.ctrl)
    }

    pub fn borrow_mut(&mut self) -> RefMut<T> {
        RefMut::map(self.0.borrow_mut(), |t| &mut t.ctrl)
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
    inner: HWndBuilder,
    _type: PhantomData<T>
}

impl<T: UsrCtrl> Drop for UserControlData<T> {
    fn drop(&mut self) {
        self.hwnd.destroy().unwrap();
    }
}

// TODO: remove T
fn prepare_hwnd<T: UsrCtrl>(mut hwnd: HWnd, create_struct: *const CREATESTRUCTW) -> *mut RefCell<UserControlData<T>> {
    unsafe {
        let p: *mut RefCell<UserControlData<T>> = mem::transmute((*create_struct).lpCreateParams);
        assert_ne!(p, ptr::null_mut());

        hwnd.set_attribute(GWLP_USERDATA, mem::transmute(p));
        (*p).borrow_mut().hwnd = hwnd;
        p
    }
}

fn user_control_proc<T: UsrCtrl>(
    hwnd: HWnd, msg: UINT, w: WPARAM, l: LPARAM
) -> Result<LRESULT> {
    let mut p: *mut RefCell<UserControlData<T>> = unsafe { mem::transmute(hwnd.get_attribute(GWLP_USERDATA)?) };
    if p == ptr::null_mut() {
        if msg != WM_NCCREATE {
            return Ok(hwnd.default_proc(msg, w, l));
        }
        p = prepare_hwnd(hwnd, unsafe { mem::transmute(l) });
    }

    let r = unsafe { &mut *p }.borrow_mut().message(hwnd, msg, w, l);
    Ok(r)
}

unsafe extern "system" fn unsafe_user_control_proc<T: UsrCtrl>(
    hwnd: HWND, msg: UINT, w: WPARAM, l: LPARAM
) -> LRESULT {
    match panic::catch_unwind(|| {
        user_control_proc::<T>(HWnd::from(hwnd), msg, w, l).unwrap()
    }) {
        Ok(result) => result,
        Err(err) => {
            PANIC.with(|p| p.set(Err(err)));
            HWnd::from(hwnd).default_proc(msg, w, l)
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

    pub fn menu(&mut self, value: HMENU) -> &mut Self {
        self.inner.menu(value);
        self
    }

    pub fn module(&mut self, value: HINSTANCE) -> &mut Self {
        self.inner.module(value);
        self
    }

    pub fn create(&mut self, ctrl: T) -> Result<UserControl<T>> {
        let mut data = Box::new(RefCell::new(UserControlData {
            hwnd: HWnd::from(ptr::null_mut()),
            ctrl
        }));
        let cell_ptr: *mut RefCell<UserControlData<T>> =  &mut *data;
        self.inner.create_param(cell_ptr as *mut ffi::c_void);
        self.inner.create()?;
        PANIC.with(|p| {
            if let Err(pnc) = p.replace(Ok(())) {
                panic::resume_unwind(pnc);
            }
        });
        Ok(UserControl(data))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use winapi::um::winuser::{WM_CREATE, WM_USER};
    use crate::sys::send_message;

    const WM_WPARAM: u32 = WM_USER;
    const WM_LPARAM: u32 = WM_USER + 1;

    struct MyControl {
        data: u32
    }

    impl UsrCtrl for MyControl {
        fn message(&mut self, hwnd: HWnd, msg: u32, w: WParam, l: LParam) -> isize {
            match msg {
                WM_CREATE => self.data = 42,
                WM_WPARAM => self.data = w.into_raw() as u32,
                WM_LPARAM => self.data = l.into_raw() as u32,
                _ => ()
            }
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
            .create(MyControl { data: 0 })
            .unwrap();
        assert_eq!(window.borrow().data, 42);

        send_message(window.as_hwnd(), WM_WPARAM, 5, 0);
        assert_eq!(window.borrow().data, 5);

        send_message(window.as_hwnd(), WM_LPARAM, 0, 28);
        assert_eq!(window.borrow().data, 28);
    }
}