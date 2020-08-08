use std::borrow::Cow;
use std::cmp;
use std::ffi::{OsStr, OsString};
use std::os::windows::ffi::{OsStrExt, OsStringExt};
use std::slice;

use std::borrow::Borrow;
use std::ops::Deref;
use std::path::PathBuf;

unsafe fn wstrlen(p: *mut u16) -> usize {
    for i in 0.. {
        if *p.offset(i) == 0 {
            return i as usize;
        }
    }
    unreachable!();
}

pub struct Wstr {
    inner: [u16],
}

impl Wstr {
    pub unsafe fn from_bytes_terminated_unchecked(s: &[u16]) -> &Wstr {
        &*(s as *const [u16] as *const Wstr)
    }

    pub fn as_ptr(&self) -> *const u16 {
        &self.inner as *const [u16] as *const u16
    }

    pub fn copy_to(&self, dest: &mut [u16]) {
        let len = cmp::min(dest.len(), self.inner.len()) - 1;
        if len > 0 {
            dest[..len].copy_from_slice(&self.inner[..len]);
            dest[len] = 0;
        }
    }
}

#[derive(Clone)]
pub struct WString {
    inner: Vec<u16>,
}

impl WString {
    pub fn new<T: Into<Vec<u16>>>(t: T) -> WString {
        Self::from_vec_unchecked(t.into())
    }

    pub fn from_vec_unchecked(mut v: Vec<u16>) -> WString {
        v.reserve_exact(1);
        v.push(0);
        WString { inner: v }
    }

    pub unsafe fn from_raw(p: *mut u16) -> WString {
        let len = wstrlen(p);
        WString::from_vec_unchecked(slice::from_raw_parts(p, len).into())
    }

    pub fn into_raw(mut self) -> *mut u16 {
        self.inner.as_mut_ptr()
    }

    pub fn into_string(self) -> Result<String, OsString> {
        <OsString as OsStringExt>::from_wide(&self.inner).into_string()
    }

    pub fn into_bytes(mut self) -> Vec<u16> {
        self.inner.pop();
        self.inner
    }

    pub fn into_bytes_terminated(self) -> Vec<u16> {
        self.inner
    }

    pub fn as_bytes(&self) -> &[u16] {
        &self.inner[..self.inner.len() - 1]
    }
    pub fn as_bytes_terminated(&self) -> &[u16] {
        &self.inner
    }

    pub fn as_wstr(&self) -> &Wstr {
        &*self
    }

    #[inline]
    pub fn from_os_str<T: AsRef<OsStr>>(s: T) -> WString {
        to_wide(s.as_ref())
    }
}

impl Deref for WString {
    type Target = Wstr;

    fn deref(&self) -> &Wstr {
        unsafe { Wstr::from_bytes_terminated_unchecked(self.as_bytes_terminated()) }
    }
}

impl From<WString> for Vec<u16> {
    fn from(s: WString) -> Self {
        s.into_bytes()
    }
}

impl Default for WString {
    fn default() -> Self {
        WString { inner: vec![0] }
    }
}

impl<'t> Default for &'t Wstr {
    fn default() -> Self {
        const EMPTY: &[u16] = &[0];
        unsafe { Wstr::from_bytes_terminated_unchecked(EMPTY) }
    }
}

fn to_wide(s: &OsStr) -> WString {
    let v: Vec<u16> = s.encode_wide().collect();
    WString::new(v)
}

impl Borrow<Wstr> for WString {
    #[inline]
    fn borrow(&self) -> &Wstr {
        self
    }
}

impl ToOwned for Wstr {
    type Owned = WString;

    #[inline]
    fn to_owned(&self) -> Self::Owned {
        WString::new(&self.inner)
    }
}

impl<'t> From<&'t Wstr> for WString {
    #[inline]
    fn from(s: &'t Wstr) -> Self {
        s.to_owned()
    }
}

// cow integration

impl<'t> From<&'t Wstr> for Cow<'t, Wstr> {
    #[inline]
    fn from(s: &'t Wstr) -> Self {
        Cow::Borrowed(s)
    }
}

impl<'t> From<&'t WString> for Cow<'t, Wstr> {
    #[inline]
    fn from(s: &'t WString) -> Self {
        Cow::Borrowed(s.as_wstr())
    }
}

impl<'t> From<WString> for Cow<'t, Wstr> {
    #[inline]
    fn from(s: WString) -> Self {
        Cow::Owned(s)
    }
}

impl<'t, T: AsRef<OsStr>> From<T> for WString {
    fn from(s: T) -> Self {
        to_wide(s.as_ref())
    }
}

// from wide

pub trait FromWide
where
    Self: Sized,
{
    fn from_wide(wide: &[u16]) -> Self;

    unsafe fn from_wide_ptr(wide: *mut u16) -> Self {
        Self::from_wide(slice::from_raw_parts(wide, wstrlen(wide)))
    }
}

impl FromWide for OsString {
    #[inline]
    fn from_wide(wide: &[u16]) -> Self {
        OsStringExt::from_wide(wide)
    }
}

impl FromWide for PathBuf {
    #[inline]
    fn from_wide(wide: &[u16]) -> Self {
        PathBuf::from(<OsString as OsStringExt>::from_wide(wide))
    }
}
