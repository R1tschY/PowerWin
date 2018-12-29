use std::error;
use std::fmt;
use std::io;
use std::ptr;
use std::ffi::c_void;

use winapi::um::errhandlingapi::{GetLastError, SetLastError};
use winapi::um::winbase::{FORMAT_MESSAGE_ALLOCATE_BUFFER, FORMAT_MESSAGE_FROM_SYSTEM, FORMAT_MESSAGE_IGNORE_INSERTS, FormatMessageW};
use winapi::um::winnt::{LANG_NEUTRAL, MAKELANGID, SUBLANG_DEFAULT};
use winapi::um::winbase::LocalFree;

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

#[derive(Debug)]
struct HError(i32);

impl error::Error for HError { }

impl fmt::Display for HError {
    fn fmt<'a>(&self, f: &mut fmt::Formatter<'a>) -> fmt::Result {
        let code = format!("0x{}", self.0 & 0x0000FFFF);
        let facility = (self.0 >> 16) & 0x00007FF;

        f.debug_struct("HRESULT")
            .field("code", &code)
            .field("facility", &facility)
            .field("message", &format_message(self.0))
            .finish()
    }
}

fn format_message(hresult: i32) { unsafe {
    let error_text: *mut u16 = ptr::null_mut();

    FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        ptr::null_mut(),
        hresult as u32,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT).into(),
        error_text,
        0,
        ptr::null_mut());

    if error_text.is_null() {
        LocalFree(error_text as *mut c_void);
    }
}}

pub fn hresult(h: i32) -> Result<bool> {
    if h < 0 {
        Err(io::Error::new(io::ErrorKind::Other, Box::new(HError(h))))
    } else {
        Ok(h == 0)  // h == S_OK
    }
}