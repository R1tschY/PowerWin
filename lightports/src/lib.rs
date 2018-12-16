extern crate winapi;

mod string;
mod error;
pub mod debug;

pub use crate::string::{WString, Wstr};
pub use crate::error::{Error, Result, result, NonNull, clear_last_error, last_os_error};