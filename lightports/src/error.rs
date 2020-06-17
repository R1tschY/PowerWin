use std::io;
use std::ptr;

use winapi::um::errhandlingapi::{GetLastError, SetLastError};

pub type Error = io::Error;
pub type Result<T> = io::Result<T>;

pub trait NonNull {
    fn non_null(&self) -> bool;
}

macro_rules! non_zero_impl {
    ($($type:ident)*) => ($(impl NonNull for $type {
        #[inline]
        fn non_null(&self) -> bool { *self != 0 }
    })*)
}

// u8 -> BOOLEAN
// i32 -> BOOL
// isize -> LONG_PTR
non_zero_impl! { u8 i32 isize }

impl<T> NonNull for *const T {
    #[inline]
    fn non_null(&self) -> bool {
        *self != ptr::null()
    }
}

impl<T> NonNull for *mut T {
    #[inline]
    fn non_null(&self) -> bool {
        *self != ptr::null_mut()
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

pub fn result<T: NonNull>(t: T) -> Result<T> {
    if !t.non_null() {
        let error = last_os_error();
        if error != 0 {
            return Err(Error::from_raw_os_error(error as i32));
        }
    }

    Ok(t)
}

pub fn void_result<T: NonNull>(t: T) -> Result<()> {
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
