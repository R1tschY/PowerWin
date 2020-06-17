use crate::{hresult, FromWide, Result};
use std::path::PathBuf;
use std::ptr;
use winapi::_core::mem::MaybeUninit;
use winapi::um::shlobj::SHGetKnownFolderPath;
use winapi::um::shtypes::KNOWNFOLDERID;

pub unsafe fn get_known_folder_path(fid: *const KNOWNFOLDERID, flags: u32) -> Result<PathBuf> {
    let mut path: MaybeUninit<*mut u16> = MaybeUninit::zeroed();
    hresult(SHGetKnownFolderPath(
        fid,
        flags,
        ptr::null_mut(),
        path.as_mut_ptr(),
    ))
    .map(|_| PathBuf::from_wide_ptr(path.assume_init()))
}
