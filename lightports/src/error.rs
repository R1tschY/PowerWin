use std::io;

use winapi::um::errhandlingapi::{GetLastError, SetLastError};

pub type Error = io::Error;
pub type Result<T> = io::Result<T>;

pub trait NonNull {
    fn non_null(&self) -> bool;
}

macro_rules! non_zero_impl {
    ($($type:ident)*) => ($(impl NonNull for $type {
        fn non_null(&self) -> bool { *self != 0 }
    })*)
}

// i32 -> BOOL
// isize -> LONG_PTR
non_zero_impl! { i32 isize }

pub fn clear_last_error() {
    unsafe { SetLastError(0); }
}

pub fn last_os_error() -> u32 {
    unsafe { GetLastError() }
}

pub fn result<T: NonNull>(t: T) -> Result<T> {
    if !t.non_null() {
        let error = last_os_error();
        if error != 0 {
            return Err(Error::from_raw_os_error(error as i32))
        }
    }

    Ok(t)
}