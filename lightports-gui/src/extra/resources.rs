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

use crate::size::Size;


pub enum ResourceRef<'t> {
    ResourceName(Cow<'t, Wstr>),
    ResourceId(u16)
}

pub enum ResourceSize {
    Resource,
    Small,
    Large,
    Specific(i32, i32)
}

impl<'t> From<u16> for ResourceRef<'t> {
    fn from(id: u16) -> Self {
        ResourceRef::ResourceId(id)
    }
}

impl<'t> From<u32> for ResourceRef<'t> {
    fn from(id: u32) -> Self {
        ResourceRef::ResourceId(id as u16)
    }
}

impl<'t> From<Cow<'t, Wstr>> for ResourceRef<'t> {
    fn from(name: Cow<'t, Wstr>) -> Self {
        ResourceRef::ResourceName(name)
    }
}

impl<'t> From<&'t Wstr> for ResourceRef<'t> {
    fn from(name: &'t Wstr) -> Self {
        ResourceRef::ResourceName(Cow::from(name))
    }
}

impl<'t> From<&str> for ResourceRef<'t> {
    fn from(name: &str) -> Self {
        ResourceRef::ResourceName(Cow::from(WString::from(name)))
    }
}