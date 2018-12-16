use crate::string::IntoWstr;
use crate::string::Wstr;
use winapi::um::debugapi::OutputDebugStringW;

pub fn output_debug_string<'t, T: IntoWstr>(s: T) {
    let wstr = s.into_wstr();
    unsafe {
        OutputDebugStringW(wstr.as_ptr());
    }
}