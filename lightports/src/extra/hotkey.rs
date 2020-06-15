use crate::{result, Result};
use winapi::um::winuser::{RegisterHotKey, UnregisterHotKey};
use winapi::shared::windef::HWND;
use crate::sys::AsHwnd;

/// for documentation: Windows RegisterHotKey function
pub struct HotKey {
    modifiers: u32,
    vk: u32,
    hwnd: HWND,
    id: i32
}

impl HotKey {
    pub fn new<H: AsHwnd>(
        modifiers: isize,
        vk: i32,
        hwnd: &H,
        id: i32
    ) -> Result<HotKey> {
        let hwnd = hwnd.as_hwnd();
        let modifiers = modifiers as u32;
        let vk = vk as u32;

        result(unsafe { RegisterHotKey(hwnd, id, modifiers, vk) })
            .map(|_| HotKey { modifiers, vk, hwnd, id } )
    }

    pub fn id(&self) -> i32 {
        self.id
    }
}

impl Drop for HotKey {
    fn drop(&mut self) {
        result(unsafe { UnregisterHotKey(self.hwnd, self.id) }).unwrap();
    }
}
