use std::ffi::{OsStr, OsString};
use std::os::windows::ffi::{OsStrExt, OsStringExt};
use std::iter::once;
use std::cmp;
use std::slice;
use std::path::PathBuf;

static EMPTY_WSTRING: EmptyWstr = EmptyWstr([0; 1]);


struct EmptyWstr(pub [u16; 1]);

impl Wstr for EmptyWstr {
    #[inline]
    fn as_ptr(&self) -> *const u16 {
        self.0.as_ptr()
    }

    #[inline]
    fn slice(&self) -> &[u16] {
        &self.0
    }
}


pub trait IntoWstr where Self::Impl : Wstr {
    type Impl;

    fn into_wstr(&self) -> Self::Impl;
}





pub trait Wstr {
    fn as_ptr(&self) -> *const u16;
    fn slice(&self) -> &[u16];
}

impl<'t> Wstr + 't {
    pub fn copy_to(&self, dest: &mut [u16]) {
        let src = self.slice();
        let len = cmp::min(dest.len(), src.len()) - 1;
        if len > 0 {
            dest[..len].copy_from_slice(&src[..len]);
            dest[len] = 0;
        }
    }

    #[inline]
    pub fn empty() -> &'static Wstr {
        &EMPTY_WSTRING
    }
}


#[derive(Clone)]
pub struct WString {
    inner: Vec<u16>
}

impl WString {
    pub fn new() -> WString {
        WString { inner: vec![0] }
    }

    #[inline]
    pub fn from_str<T: AsRef<OsStr>>(s: T) -> WString {
        WString { inner: to_wide(s.as_ref()) }
    }
}

impl Wstr for WString {
    fn as_ptr(&self) -> *const u16 {
        self.inner.as_ptr()
    }

    fn slice(&self) -> &[u16] {
        &self.inner
    }
}

impl Wstr for &WString {
    fn as_ptr(&self) -> *const u16 {
        self.inner.as_ptr()
    }

    fn slice(&self) -> &[u16] {
        &self.inner
    }
}

fn to_wide(s: &OsStr) -> Vec<u16> {
    let mut v: Vec<u16> = s.encode_wide().collect();
    v.push(0);
    v
}


impl<'t> IntoWstr for &'t WString {
    type Impl = &'t WString;

    fn into_wstr(&self) -> &'t WString {
        self
    }
}

impl IntoWstr for &str {
    type Impl = WString;

    #[inline]
    fn into_wstr(&self) -> WString {
        WString::from(&to_wide(self.as_ref()) as &[u16])
    }
}

impl<'t> From<&'t Wstr> for WString {
    #[inline]
    fn from(src: &'t Wstr) -> Self {
        WString::from(src.slice())
    }
}

impl From<&[u16]> for WString {
    fn from(slice: &[u16]) -> Self {
        let inner: Vec<u16> = if let Some(last) = slice.last() {
            if *last == 0 {
                Vec::from(slice)
            } else {
                slice.iter().cloned().chain(once(0)).collect()
            }
        } else {
            vec![0]
        };

        WString { inner }
    }
}

pub trait FromWide where Self: Sized {
    fn from_wide(wide: &[u16]) -> Self;

    fn from_wide_ptr(wide: *const u16) -> Self { unsafe {
        for i in 0.. {
            if *wide.offset(i) == 0 {
                return Self::from_wide(slice::from_raw_parts(wide, i as usize));
            }
        }
        unreachable!();
    }}
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



