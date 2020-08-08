use crate::{WinResult, Wstr};
use bitflags::bitflags;
use std::{io, mem, ptr};
use winapi::shared::minwindef::TRUE;
use winapi::shared::ntdef::LPCWSTR;
use winapi::shared::windef::HMENU;
use winapi::um::winuser::{
    AppendMenuW, CheckMenuItem, CreateMenu, CreatePopupMenu, DeleteMenu, DestroyMenu, GetMenuState,
    InsertMenuW, IsMenu, MF_BYCOMMAND, MF_BYPOSITION, MF_CHECKED, MF_DISABLED, MF_ENABLED,
    MF_GRAYED, MF_POPUP, MF_SEPARATOR, MF_STRING, MF_UNCHECKED,
};

const MINUS_ONE: u32 = -1i32 as u32;

bitflags! {
    #[derive(Default)]
    pub struct MenuEntryFlags: u32 {
        const CHECKED = MF_CHECKED;
        const DISABLED = MF_DISABLED;
        const ENABLED = MF_ENABLED;
        const GRAYED = MF_GRAYED;
        const UNCHECKED = MF_UNCHECKED;
    }
}

pub enum MenuItem<'a> {
    Seperator,
    Menu(&'a Wstr, &'a dyn MenuTrait),
    TextEntry(&'a Wstr, u32),
}

pub trait MenuTrait {
    fn as_hmenu(&self) -> HMENU;

    fn is_valid(&self) -> bool {
        self.as_hmenu() != ptr::null_mut() && unsafe { IsMenu(self.as_hmenu()) == TRUE }
    }

    fn contains(&self, command_id: u32) -> bool {
        get_state(self.as_hmenu(), command_id, MF_BYCOMMAND).is_some()
    }

    fn add(&self, item: MenuItem, flags: MenuEntryFlags) -> io::Result<()> {
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

    fn insert_at(&self, pos: u32, item: MenuItem, flags: MenuEntryFlags) -> io::Result<()> {
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

    fn insert_before(&self, command: u32, item: MenuItem, flags: MenuEntryFlags) -> io::Result<()> {
        unsafe {
            let (flags_item, idnew_item, new_item) = get_create_params(item);

            InsertMenuW(
                self.as_hmenu(),
                command,
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

    fn delete(&self, command: u32) -> io::Result<()> {
        unsafe { DeleteMenu(self.as_hmenu(), command, MF_BYCOMMAND).into_void_result() }
    }

    fn is_checked(&self, command: u32) -> Option<bool> {
        get_state(self.as_hmenu(), command, MF_BYCOMMAND).map(|s| s & MF_CHECKED != 0)
    }

    fn is_checked_at(&self, pos: u32) -> Option<bool> {
        get_state(self.as_hmenu(), pos, MF_BYPOSITION).map(|s| s & MF_CHECKED != 0)
    }

    fn is_enabled(&self, command: u32) -> Option<bool> {
        get_state(self.as_hmenu(), command, MF_BYCOMMAND)
            .map(|s| s & (MF_DISABLED | MF_GRAYED) != 0)
    }

    fn is_enabled_at(&self, pos: u32) -> Option<bool> {
        get_state(self.as_hmenu(), pos, MF_BYPOSITION).map(|s| s & (MF_DISABLED | MF_GRAYED) != 0)
    }

    fn set_checked(&self, command: u32, value: bool) -> Option<u32> {
        unsafe {
            state_result(CheckMenuItem(
                self.as_hmenu(),
                command,
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

    // fn open(&self, pt: Point, window: impl AsHwnd) {
    //     ::TrackPopupMenu(self.as_hmenu,
    //                      TPM_LEFTALIGN | TPM_RIGHTBUTTON,
    //                      pt.getX(), pt.getY(), 0, window, nullptr);
    // }
}

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

impl MenuTrait for MenuView {
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

impl MenuTrait for Menu {
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
