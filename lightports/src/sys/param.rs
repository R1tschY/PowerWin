use std::mem;

use winapi::shared::minwindef::{HIWORD, LOWORD};
use winapi::shared::windowsx::{GET_X_LPARAM, GET_Y_LPARAM};

#[derive(Copy, Clone, Debug)]
pub struct WParam(usize);
#[derive(Copy, Clone, Debug)]
pub struct LParam(isize);
#[derive(Copy, Clone, Debug)]
pub struct LResult(isize);

impl WParam {
    pub fn as_raw(&self) -> usize {
        self.0
    }

    pub unsafe fn as_ptr<T>(&self) -> *const T {
        mem::transmute(self.0)
    }

    pub unsafe fn as_mut_ptr<T>(&self) -> *mut T {
        mem::transmute(self.0)
    }

    pub fn low_word(&self) -> u16 {
        LOWORD(self.0 as u32)
    }
    pub fn high_word(&self) -> u16 {
        HIWORD(self.0 as u32)
    }

    pub fn get_x(&self) -> i32 {
        GET_X_LPARAM(unsafe { mem::transmute(self.0) })
    }
    pub fn get_y(&self) -> i32 {
        GET_Y_LPARAM(unsafe { mem::transmute(self.0) })
    }
}

impl LParam {
    pub fn as_raw(&self) -> isize {
        self.0
    }

    pub unsafe fn as_ptr<T>(&self) -> *const T {
        mem::transmute(self.0)
    }

    pub unsafe fn as_mut_ptr<T>(&self) -> *mut T {
        mem::transmute(self.0)
    }

    pub fn low_word(&self) -> u16 {
        LOWORD(self.0 as u32)
    }
    pub fn high_word(&self) -> u16 {
        HIWORD(self.0 as u32)
    }

    pub fn get_x(&self) -> i32 {
        GET_X_LPARAM(self.0)
    }
    pub fn get_y(&self) -> i32 {
        GET_Y_LPARAM(self.0)
    }
}

impl LResult {
    pub fn as_raw(&self) -> isize {
        self.0
    }

    pub fn as_ptr<T>(&self) -> *const T {
        unsafe { mem::transmute(self.0) }
    }

    pub fn as_mut_ptr<T>(&self) -> *mut T {
        unsafe { mem::transmute(self.0) }
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
