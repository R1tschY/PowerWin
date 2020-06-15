use crate::sys::{LParam, LResult, WParam, Window};
use crate::usr_ctrl::UsrCtrl;
use std::collections::HashMap;

pub enum WindowMsg<T: Fn() -> () + 'static> {
    WmCreate(T),
    WmDestroy(T),
}
pub use self::WindowMsg::*;

trait MessageHandlerBuilder {
    fn message(&self) -> u32;
    fn create(self) -> Box<dyn Fn(WParam, LParam) -> Option<LResult>>;
}

pub struct CustomControl {
    connections: HashMap<u32, Box<dyn Fn(WParam, LParam) -> Option<LResult>>>,
}

impl CustomControl {
    fn connect<T: MessageHandlerBuilder>(&mut self, t: T) {
        self.connections.insert(t.message(), t.create());
    }
}

impl UsrCtrl for CustomControl {
    type CreateParam = ();

    fn create(_hwnd: Window, _params: &()) -> Self {
        CustomControl {
            connections: HashMap::new(),
        }
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        if let Some(ref func) = self.connections.get(&msg) {
            if let Some(res) = func(w, l) {
                return res;
            }
        }
        hwnd.default_proc(msg, w, l)
    }
}

impl<T: Fn() -> () + 'static> MessageHandlerBuilder for WindowMsg<T> {
    #[inline]
    fn message(&self) -> u32 {
        use winapi::um::winuser::*;

        match self {
            WmCreate(_) => WM_CREATE,
            WmDestroy(_) => WM_DESTROY,
        }
    }

    #[inline]
    fn create(self) -> Box<dyn Fn(WParam, LParam) -> Option<LResult>> {
        match self {
            WmCreate(func) => Box::new(move |_w, _l| {
                func();
                None
            }),
            WmDestroy(func) => Box::new(move |_w, _l| {
                func();
                None
            }),
        }
    }
}
