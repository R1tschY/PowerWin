use crate::error::WinResult;
use crate::{result, FromWide, WString};
use std::ffi::{c_void, OsStr, OsString};
use std::{io, ptr};
use winapi::_core::mem::MaybeUninit;
use winapi::shared::minwindef::HKEY;
use winapi::shared::winerror::{ERROR_FILE_NOT_FOUND, ERROR_SUCCESS};
use winapi::um::winnt::{KEY_ALL_ACCESS, REG_OPTION_NON_VOLATILE, REG_SZ};
use winapi::um::winreg::{
    RegCloseKey, RegCreateKeyExW, RegDeleteValueW, RegGetValueW, RegSetValueExW, HKEY_CURRENT_USER,
    RRF_RT_REG_SZ,
};

struct KeyHandle(HKEY);

impl Drop for KeyHandle {
    fn drop(&mut self) {
        unsafe {
            RegCloseKey(self.0);
        }
    }
}

unsafe fn open_autostart_key() -> io::Result<KeyHandle> {
    let mut key: MaybeUninit<HKEY> = MaybeUninit::uninit();
    result(RegCreateKeyExW(
        HKEY_CURRENT_USER,
        WString::from_os_str("Software\\Microsoft\\Windows\\CurrentVersion\\Run").as_ptr(),
        0,
        ptr::null_mut(),
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        ptr::null_mut(),
        key.as_mut_ptr(),
        ptr::null_mut(),
    ))?;
    Ok(KeyHandle(key.assume_init()))
}

pub fn set_to_autostart(app_id: &str, app_path: &OsStr) -> io::Result<()> {
    unsafe {
        let key = open_autostart_key()?;
        let value = WString::from_os_str(app_path);

        RegSetValueExW(
            key.0,
            WString::from_os_str(app_id).as_ptr(),
            0,
            REG_SZ,
            value.as_ptr() as *const _ as *const u8,
            value.bytes_len() as u32,
        )
        .into_void_result()
    }
}

pub fn remove_from_autostart(app_id: &str) -> io::Result<()> {
    unsafe {
        let key = open_autostart_key()?;

        let error = RegDeleteValueW(key.0, WString::from_os_str(app_id).as_ptr());
        if error != ERROR_FILE_NOT_FOUND as i32 {
            Err(io::Error::from_raw_os_error(error))
        } else {
            Ok(())
        }
    }
}

pub fn get_autostart_path(app_id: &str) -> io::Result<Option<OsString>> {
    unsafe {
        let key = open_autostart_key()?;

        let mut buffer: Vec<u16> = vec![];
        let mut buffersize: u32 = 0;
        let mut error = RegGetValueW(
            key.0,
            ptr::null_mut(),
            WString::from_os_str(app_id).as_ptr(),
            RRF_RT_REG_SZ,
            ptr::null_mut(),
            ptr::null_mut(),
            &mut buffersize,
        );
        if error == ERROR_SUCCESS as i32 {
            buffer.resize(buffersize as usize, 0);
            error = RegGetValueW(
                key.0,
                ptr::null_mut(),
                WString::from_os_str(app_id).as_ptr(),
                RRF_RT_REG_SZ,
                ptr::null_mut(),
                buffer.as_mut_ptr() as *mut c_void,
                &mut buffersize,
            )
        }

        if error != ERROR_SUCCESS as i32 {
            if error == ERROR_FILE_NOT_FOUND as i32 {
                Ok(None)
            } else {
                Err(io::Error::from_raw_os_error(error))
            }
        } else {
            Ok(Some(OsString::from_wide(&buffer)))
        }
    }
}
