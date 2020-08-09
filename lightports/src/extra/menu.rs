use std::{io, mem, ptr};

use bitflags::bitflags;
use winapi::shared::minwindef::TRUE;
use winapi::shared::ntdef::LPCWSTR;
use winapi::shared::windef::HMENU;
use winapi::um::winuser::{
    AppendMenuW, CheckMenuItem, CreateMenu, CreatePopupMenu, DeleteMenu, DestroyMenu, GetMenuState,
    InsertMenuW, IsMenu, TrackPopupMenu, MF_BYCOMMAND, MF_BYPOSITION, MF_CHECKED, MF_DISABLED,
    MF_ENABLED, MF_GRAYED, MF_POPUP, MF_SEPARATOR, MF_STRING, MF_UNCHECKED, TPM_BOTTOMALIGN,
    TPM_CENTERALIGN, TPM_HORNEGANIMATION, TPM_HORPOSANIMATION, TPM_LEFTALIGN, TPM_LEFTBUTTON,
    TPM_NOANIMATION, TPM_NONOTIFY, TPM_RIGHTALIGN, TPM_RIGHTBUTTON, TPM_TOPALIGN, TPM_VCENTERALIGN,
    TPM_VERNEGANIMATION, TPM_VERPOSANIMATION,
};

use crate::def::point::Point;
use crate::sys::AsHwnd;
use crate::{WString, WinResult};

const MINUS_ONE: u32 = -1i32 as u32;

bitflags! {
    #[derive(Default)]
    pub struct MenuFlags: u32 {
        const CHECKED = MF_CHECKED;
        const DISABLED = MF_DISABLED;
        const ENABLED = MF_ENABLED;
        const GRAYED = MF_GRAYED;
        const UNCHECKED = MF_UNCHECKED;
    }
}

bitflags! {
    #[derive(Default)]
    pub struct TrackFlags: u32 {
        const CENTERALIGN = TPM_CENTERALIGN;
        const LEFTALIGN = TPM_LEFTALIGN;
        const RIGHTALIGN = TPM_RIGHTALIGN;
        const BOTTOMALIGN = TPM_BOTTOMALIGN;
        const TOPALIGN = TPM_TOPALIGN;
        const VCENTERALIGN = TPM_VCENTERALIGN;
        const NONOTIFY = TPM_NONOTIFY;
        const LEFTBUTTON = TPM_LEFTBUTTON;
        const RIGHTBUTTON = TPM_RIGHTBUTTON;
        const HORNEGANIMATION = TPM_HORNEGANIMATION;
        const HORPOSANIMATION = TPM_HORPOSANIMATION;
        const NOANIMATION = TPM_NOANIMATION;
        const VERNEGANIMATION = TPM_VERNEGANIMATION;
        const VERPOSANIMATION = TPM_VERPOSANIMATION;
    }
}

pub enum MenuItem<'a> {
    Seperator,
    Menu(WString, &'a dyn AsHmenu),
    TextEntry(WString, u16),
}

pub trait AsHmenu {
    fn as_hmenu(&self) -> HMENU;
}

pub trait MenuFunctions: AsHmenu {
    fn is_valid(&self) -> bool {
        self.as_hmenu() != ptr::null_mut() && unsafe { IsMenu(self.as_hmenu()) == TRUE }
    }

    fn contains(&self, command_id: u16) -> bool {
        get_state(self.as_hmenu(), command_id as u32, MF_BYCOMMAND).is_some()
    }

    fn add(&self, item: MenuItem, flags: MenuFlags) -> io::Result<()> {
        unsafe {
            let (flags_item, idnew_item, new_item) = get_create_params(item);
            AppendMenuW(
                self.as_hmenu(),
                flags_item | flags.bits,
                idnew_item,
                new_item,
            )
            .into_void_result()
        }
    }

    fn insert_at(&self, pos: u32, item: MenuItem, flags: MenuFlags) -> io::Result<()> {
        unsafe {
            let (flags_item, idnew_item, new_item) = get_create_params(item);

            InsertMenuW(
                self.as_hmenu(),
                pos,
                MF_BYPOSITION | flags_item | flags.bits,
                idnew_item,
                new_item,
            )
            .into_void_result()
        }
    }

    fn insert_before(&self, command: u16, item: MenuItem, flags: MenuFlags) -> io::Result<()> {
        unsafe {
            let (flags_item, idnew_item, new_item) = get_create_params(item);

            InsertMenuW(
                self.as_hmenu(),
                command as u32,
                MF_BYCOMMAND | flags_item | flags.bits,
                idnew_item,
                new_item,
            )
            .into_void_result()
        }
    }

    fn delete_at(&self, pos: u32) -> io::Result<()> {
        unsafe { DeleteMenu(self.as_hmenu(), pos, MF_BYPOSITION).into_void_result() }
    }

    fn delete(&self, command: u16) -> io::Result<()> {
        unsafe { DeleteMenu(self.as_hmenu(), command as u32, MF_BYCOMMAND).into_void_result() }
    }

    fn is_checked(&self, command: u16) -> Option<bool> {
        get_state(self.as_hmenu(), command as u32, MF_BYCOMMAND).map(|s| s & MF_CHECKED != 0)
    }

    fn is_checked_at(&self, pos: u32) -> Option<bool> {
        get_state(self.as_hmenu(), pos, MF_BYPOSITION).map(|s| s & MF_CHECKED != 0)
    }

    fn is_enabled(&self, command: u16) -> Option<bool> {
        get_state(self.as_hmenu(), command as u32, MF_BYCOMMAND)
            .map(|s| s & (MF_DISABLED | MF_GRAYED) != 0)
    }

    fn is_enabled_at(&self, pos: u32) -> Option<bool> {
        get_state(self.as_hmenu(), pos, MF_BYPOSITION).map(|s| s & (MF_DISABLED | MF_GRAYED) != 0)
    }

    fn set_checked(&self, command: u16, value: bool) -> Option<u32> {
        unsafe {
            state_result(CheckMenuItem(
                self.as_hmenu(),
                command as u32,
                MF_BYCOMMAND | (if value { MF_CHECKED } else { MF_UNCHECKED }),
            ))
        }
    }

    fn set_checked_at(&self, pos: u32, value: bool) -> Option<u32> {
        unsafe {
            state_result(CheckMenuItem(
                self.as_hmenu(),
                pos,
                MF_BYPOSITION | (if value { MF_CHECKED } else { MF_UNCHECKED }),
            ))
        }
    }

    fn track_popup_menu(
        &self,
        flags: TrackFlags,
        pt: Point,
        window: impl AsHwnd,
    ) -> io::Result<()> {
        unsafe {
            TrackPopupMenu(
                self.as_hmenu(),
                flags.bits(),
                pt.x,
                pt.y,
                0,
                window.as_hwnd(),
                ptr::null_mut(),
            )
            .into_void_result()
        }
    }
}

impl<T: AsHmenu> MenuFunctions for T {}

fn get_create_params(item: MenuItem) -> (u32, usize, LPCWSTR) {
    match item {
        MenuItem::Seperator => (MF_SEPARATOR, 0, ptr::null_mut()),
        MenuItem::Menu(name, submenu) => (
            MF_POPUP | MF_STRING,
            unsafe { mem::transmute(submenu.as_hmenu()) },
            name.as_ptr(),
        ),
        MenuItem::TextEntry(name, id) => (MF_STRING, id as usize, name.as_ptr()),
    }
}

fn get_state(menu: HMENU, id: u32, flags: u32) -> Option<u32> {
    match unsafe { GetMenuState(menu, id, flags) } {
        MINUS_ONE => None,
        state => Some(state),
    }
}

fn state_result(result: u32) -> Option<u32> {
    match result {
        MINUS_ONE => None,
        state => Some(state),
    }
}

pub struct MenuView(HMENU);
pub struct Menu(HMENU);

impl AsHmenu for MenuView {
    fn as_hmenu(&self) -> HMENU {
        self.0
    }
}

impl From<HMENU> for MenuView {
    fn from(value: HMENU) -> Self {
        MenuView(value)
    }
}

impl Menu {
    pub fn new() -> io::Result<Menu> {
        unsafe { CreateMenu().into_result().map(|e| e.into()) }
    }

    pub fn new_popup() -> io::Result<Menu> {
        unsafe { CreatePopupMenu().into_result().map(|e| e.into()) }
    }
}

impl AsHmenu for Menu {
    fn as_hmenu(&self) -> HMENU {
        self.0
    }
}

impl Drop for Menu {
    fn drop(&mut self) {
        unsafe {
            DestroyMenu(self.0);
        }
    }
}

impl From<HMENU> for Menu {
    fn from(value: HMENU) -> Self {
        Menu(value)
    }
}
