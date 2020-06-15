use winapi::um::shlobj::SHGetKnownFolderPath;
use std::ptr;
use crate::{hresult, Result, FromWide};
use std::path::PathBuf;
use winapi::um::shtypes::KNOWNFOLDERID;

pub fn get_known_folder_path(fid: *const KNOWNFOLDERID, flags: u32) -> Result<PathBuf> {
    let mut path: *mut u16 = 0 as *mut u16;
    unsafe {
        hresult(SHGetKnownFolderPath(fid, flags, ptr::null_mut(), &mut path))
            .map(|_| { PathBuf::from_wide_ptr(path) })
    }
}