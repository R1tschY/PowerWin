extern crate winapi;

mod string;
mod error;
pub mod debug;

pub use crate::string::{WString, Wstr, FromWide};
pub use crate::error::{Error, Result, result, hresult, NonNull, clear_last_error, last_os_error};