use std::mem;

pub struct WParam(usize);
pub struct LParam(isize);

impl WParam {
    pub fn into_raw(&self) -> usize { return self.0 }

    pub unsafe fn as_ptr<T>(&self) -> *const T {
        mem::transmute(self.0)
    }

    pub unsafe fn as_mut_ptr<T>(&self) -> *mut T {
        mem::transmute(self.0)
    }
}

impl LParam {
    pub fn into_raw(&self) -> isize { return self.0 }

    pub unsafe fn as_ptr<T>(&self) -> *const T {
        mem::transmute(self.0)
    }

    pub unsafe fn as_mut_ptr<T>(&self) -> *mut T {
        mem::transmute(self.0)
    }
}

impl From<WParam> for usize {
    fn from(x: WParam) -> Self {
        x.0
    }
}

impl From<usize> for WParam {
    fn from(x: usize) -> Self {
        WParam(x)
    }
}

impl From<LParam> for isize {
    fn from(x: LParam) -> Self {
        x.0
    }
}

impl From<isize> for LParam {
    fn from(x: isize) -> Self {
        LParam(x)
    }
}