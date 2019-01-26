use std::borrow::Cow;
use std::ops::Deref;
use std::ptr;

use lightports::{result, Result};
use lightports::{Wstr, WString};
use winapi::shared::minwindef::HINSTANCE;
use winapi::shared::windef::HICON;
use winapi::um::winuser::DestroyIcon;
use winapi::um::winuser::IMAGE_ICON;
use winapi::um::winuser::LoadImageW;
use winapi::um::winuser::LR_DEFAULTSIZE;
use winapi::um::winuser::LR_SHARED;
use winapi::um::winuser::MAKEINTRESOURCEW;
use winapi::um::commctrl::LoadIconMetric;
use winapi::um::commctrl::LIM_SMALL;
use winapi::um::commctrl::LIM_LARGE;

use crate::size::Size;
use crate::extra::resources::{ResourceRef, ResourceSize};

pub struct Icon(HICON);

impl Icon {
    pub fn create<'t>(
        hinst: HINSTANCE, res: ResourceRef<'t>, size: ResourceSize, flags: u32
    ) -> Result<Icon> {
        let name = match res {
            ResourceRef::ResourceName(name) => name.as_ptr(),
            ResourceRef::ResourceId(id) => MAKEINTRESOURCEW(id)
        };
        let mut icon_ret: HICON = ptr::null_mut();

        let ret = match size {
            ResourceSize::Small =>
                unsafe { LoadIconMetric(hinst, name, LIM_SMALL as i32, &mut icon_ret); icon_ret },
            ResourceSize::Large =>
                unsafe { LoadIconMetric(hinst, name, LIM_LARGE as i32, &mut icon_ret); icon_ret },
            ResourceSize::Resource =>
                unsafe { LoadImageW(hinst, name, IMAGE_ICON, 0, 0, flags) as HICON },
            ResourceSize::Specific(w, h) =>
                unsafe { LoadImageW(hinst, name, IMAGE_ICON, w, h, flags) as HICON },
        };

        ;
        Ok(Icon(result(ret)?))
    }

    pub fn from_resource<'t, R: Into<ResourceRef<'t>>>(
        hinst: HINSTANCE, res: R, size: ResourceSize
    ) -> Result<OwnedIcon> {
        assert!(hinst != ptr::null_mut());
        Self::create(hinst, res.into(), size, 0).map(|i| OwnedIcon(i))
    }

    pub fn from_resource_shared<'t, R: Into<ResourceRef<'t>>>(
        hinst: HINSTANCE, res: R, size: ResourceSize
    ) -> Result<Icon> {
        assert!(hinst != ptr::null_mut());
        Self::create(hinst, res.into(), size, LR_SHARED)
    }

    pub fn as_raw(&self) -> HICON {
        self.0
    }
}


pub struct OwnedIcon(Icon);

impl Drop for OwnedIcon {
    fn drop(&mut self) {
        unsafe { DestroyIcon(self.0.as_raw()) };
    }
}

impl Deref for OwnedIcon {
    type Target = Icon;

    fn deref(&self) -> &Icon {
        &self.0
    }
}
