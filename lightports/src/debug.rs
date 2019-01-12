use crate::string::WString;
use std::borrow::Cow;
use winapi::um::debugapi::OutputDebugStringW;

pub fn output_debug_string(s: &str) {
    unsafe {
        OutputDebugStringW(WString::from(s).as_ptr());
    }
}