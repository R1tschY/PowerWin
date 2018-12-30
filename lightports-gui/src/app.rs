use std::ptr;
use winapi::shared::minwindef::HINSTANCE;
use winapi::um::libloaderapi::GetModuleHandleW;

pub fn app_instance() -> HINSTANCE {
    unsafe { GetModuleHandleW(ptr::null_mut()) }
}