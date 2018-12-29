use std::mem;

use winapi::shared::minwindef::{LOWORD, HIWORD};
use winapi::shared::windowsx::{GET_X_LPARAM, GET_Y_LPARAM};

#[derive(Copy, Clone)] pub struct WParam(usize);
#[derive(Copy, Clone)] pub struct LParam(isize);
#[derive(Copy, Clone)] pub struct LResult(isize);

impl WParam {
    pub fn into_raw(&self) -> usize { return self.0 }

    pub unsafe fn as_ptr<T>(&self) -> *const T {
        mem::transmute(self.0)
    }

    pub unsafe fn as_mut_ptr<T>(&self) -> *mut T {
        mem::transmute(self.0)
    }

    pub fn low_word(&self) -> u16 { LOWORD(self.0 as u32) }
    pub fn high_word(&self) -> u16 { HIWORD(self.0 as u32) }

    pub fn get_x(&self) -> i32 { GET_X_LPARAM(unsafe { mem::transmute(self.0) }) }
    pub fn get_y(&self) -> i32 { GET_Y_LPARAM(unsafe { mem::transmute(self.0) }) }
}

impl LParam {
    pub fn into_raw(&self) -> isize { return self.0 }

    pub unsafe fn as_ptr<T>(&self) -> *const T {
        mem::transmute(self.0)
    }

    pub unsafe fn as_mut_ptr<T>(&self) -> *mut T {
        mem::transmute(self.0)
    }

    pub fn low_word(&self) -> u16 { LOWORD(self.0 as u32) }
    pub fn high_word(&self) -> u16 { HIWORD(self.0 as u32) }

    pub fn get_x(&self) -> i32 { GET_X_LPARAM(self.0) }
    pub fn get_y(&self) -> i32 { GET_Y_LPARAM(self.0) }
}

impl LResult {
    pub fn into_raw(&self) -> isize { return self.0 }

    pub unsafe fn as_ptr<T>(&self) -> *const T {
        mem::transmute(self.0)
    }

    pub unsafe fn as_mut_ptr<T>(&self) -> *mut T {
        mem::transmute(self.0)
    }
}

impl From<WParam> for usize {
    fn from(x: WParam) -> Self {
        x.0
    }
}

impl From<usize> for WParam {
    fn from(x: usize) -> Self {
        WParam(x)
    }
}

impl From<LParam> for isize {
    fn from(x: LParam) -> Self {
        x.0
    }
}

impl From<isize> for LParam {
    fn from(x: isize) -> Self {
        LParam(x)
    }
}

impl From<LResult> for isize {
    fn from(x: LResult) -> Self {
        x.0
    }
}

impl From<isize> for LResult {
    fn from(x: isize) -> Self {
        LResult(x)
    }
}