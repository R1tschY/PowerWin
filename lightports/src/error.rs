use std::io;
use std::ptr;

use winapi::um::errhandlingapi::{GetLastError, SetLastError};

pub type Error = io::Error;
pub type Result<T> = io::Result<T>;

pub trait WinResult: Sized {
    fn is_null(&self) -> bool;

    fn into_result(self) -> Result<Self> {
        result(self)
    }

    fn into_void_result(self) -> Result<()> {
        void_result(self)
    }

    fn into_option(self) -> Option<Self> {
        if self.is_null() {
            None
        } else {
            Some(self)
        }
    }
}

macro_rules! non_zero_impl {
    ($($type:ident)*) => ($(impl WinResult for $type {
        #[inline]
        fn is_null(&self) -> bool { *self == 0 }
    })*)
}

// u8 -> BOOLEAN
// i32 -> BOOL
// isize -> LONG_PTR
non_zero_impl! { u8 i32 isize }

impl<T> WinResult for *const T {
    #[inline]
    fn is_null(&self) -> bool {
        *self == ptr::null()
    }
}

impl<T> WinResult for *mut T {
    #[inline]
    fn is_null(&self) -> bool {
        *self == ptr::null_mut()
    }
}

#[inline]
pub fn clear_last_error() {
    unsafe {
        SetLastError(0);
    }
}

#[inline]
pub fn last_os_error() -> u32 {
    unsafe { GetLastError() }
}

pub fn result<T: WinResult>(t: T) -> Result<T> {
    if t.is_null() {
        let error = last_os_error();
        if error != 0 {
            clear_last_error();
            return Err(Error::from_raw_os_error(error as i32));
        }
    }

    Ok(t)
}

pub fn void_result<T: WinResult>(t: T) -> Result<()> {
    result(t).map(|_| ())
}

#[inline]
pub fn hresult(h: i32) -> Result<bool> {
    if h < 0 {
        Err(Error::from_raw_os_error(h))
    } else {
        Ok(h == 0) // h == S_OK
    }
}
