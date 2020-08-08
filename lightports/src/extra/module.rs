use std::ptr;
use winapi::shared::minwindef::HMODULE;
use winapi::um::libloaderapi::GetModuleHandleW;

pub fn application_module() -> HMODULE {
    unsafe { GetModuleHandleW(ptr::null_mut()) }
}
