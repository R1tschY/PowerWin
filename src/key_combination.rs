use std::num;

use nom::{is_digit, digit, ErrorKind, Err as Error, space0};
use nom::types::CompleteStr;
use winapi::um::winuser::MOD_ALT;
use winapi::um::winuser::MOD_CONTROL;
use winapi::um::winuser::MOD_SHIFT;
use winapi::um::winuser::MOD_WIN;
use winapi::um::winuser::VK_F1;
use std::io;
use winapi::um::winuser::VkKeyScanExW;
use winapi::um::winuser::GetKeyboardLayout;
use winapi::um::winuser::VK_BACK;
use winapi::um::winuser::VK_HOME;
use winapi::um::winuser::VK_END;
use winapi::um::winuser::VK_NEXT;
use winapi::um::winuser::VK_PRIOR;
use winapi::um::winuser::VK_DELETE;
use winapi::um::winuser::VK_INSERT;
use winapi::um::winuser::VK_ESCAPE;
use winapi::um::winuser::VK_RETURN;

#[derive(Eq, PartialEq, Debug, Copy, Clone)]
pub enum Key {
    Ctrl,
    Alt,
    Shift,
    Win,
    F(u8),
    Char(char),
    Backspace, Delete, Home, End, PageUp, PageDown, Insert, Esc, Enter
}

fn create_function_key(number: CompleteStr) -> Result<Key, num::ParseIntError> {
    // TODO: test for <= 24
    Ok(Key::F(u8::from_str_radix(number.as_ref(), 10)?))
}

fn is_ascii_digit(d: char) -> bool {
    d.is_ascii_digit()
}

named!(fkey<CompleteStr, Key>,
    map_res!(
        preceded!(tag_no_case!("f"), take_while1!(is_ascii_digit)),
        create_function_key
    )
);

named!(modifier<CompleteStr, Key>,
    alt!(
        value!(Key::Ctrl, tag_no_case!("ctrl")) |
        value!(Key::Alt, tag_no_case!("alt")) |
        value!(Key::Win, tag_no_case!("win")) |
        value!(Key::Shift, tag_no_case!("shift"))
    )
);

named!(special_key<CompleteStr, Key>,
    alt!(
        value!(Key::Char(' '), tag_no_case!("space")) |
        value!(Key::Char('\t'), tag_no_case!("tab")) |
        value!(Key::Enter, tag_no_case!("enter")) |
        value!(Key::Backspace, tag_no_case!("backspace")) |
        value!(Key::Home, tag_no_case!("home")) |
        value!(Key::End, tag_no_case!("end")) |
        value!(Key::PageUp, tag_no_case!("page_up")) |
        value!(Key::PageDown, tag_no_case!("page_down")) |
        value!(Key::Esc, tag_no_case!("esc"))
    )
);

named!(key<CompleteStr, Key>,
    alt!(modifier | fkey | special_key)
);

named!(key_combo<CompleteStr, Vec<Key> >,
    exact!(separated_nonempty_list_complete!(
        tag!("+"), delimited!(space0, key, space0)
    ))
);

pub fn parse_key_combination(input: &str) -> io::Result<Vec<Key>> {
    // TODO: check for multiple same keys
    // TODO: check for only one key
    match key_combo(input.into()) {
        Ok((_, result)) => Ok(result),
        Err(_) => Err(io::Error::new(io::ErrorKind::InvalidInput, "invalid key combination")),
    }
}

fn char_to_vk(c: &char) -> io::Result<(u32, u32)> {
    let mut b = [0; 2];
    let utf16 = c.encode_utf16(&mut b);
    if utf16.len() == 2 {
        Err(io::Error::new(io::ErrorKind::InvalidInput,
                           "unsupported character key (two utf16 characters needed)"))
    } else {
        let res = unsafe { VkKeyScanExW(utf16[0].clone(), GetKeyboardLayout(0)) };
        if res == -1 {
            return Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "unsupported character key (not supported in current input locale)"))
        }

        let mut modifier = 0;
        if res & 0x100 != 0 { modifier |= MOD_SHIFT; }
        if res & 0x200 != 0 { modifier |= MOD_CONTROL; }
        if res & 0x400 != 0 { modifier |= MOD_ALT; }
        if (res >> 8) >= 8 {
            Err(io::Error::new(io::ErrorKind::InvalidInput,
                               "unsupported character key (unsupported modifier required)"))
        } else {
            Ok((modifier as u32, (res & 0xFF) as u32))
        }
    }
}

pub fn to_vk(key_seq: &[Key]) -> io::Result<(u32, u32)> {
    let mut modifier = 0u32;
    let mut vk = 0u32;

    for key in key_seq {
        match key {
            Key::Alt => modifier |= MOD_ALT as u32,
            Key::Ctrl => modifier |= MOD_CONTROL as u32,
            Key::Shift => modifier |= MOD_SHIFT as u32,
            Key::Win => modifier |= MOD_WIN as u32,
            Key::F(f) => vk = VK_F1 as u32 + (f - 1) as u32,
            Key::Char(c) => {
                let (m, k) = char_to_vk(c)?;
                modifier |= m;
                vk = k;
            },
            Key::Backspace => vk = VK_BACK as u32,
            Key::Home => vk = VK_HOME as u32,
            Key::End => vk = VK_END as u32,
            Key::PageDown => vk = VK_NEXT as u32,
            Key::PageUp => vk = VK_PRIOR as u32,
            Key::Delete => vk = VK_DELETE as u32,
            Key::Insert => vk = VK_INSERT as u32,
            Key::Esc => vk = VK_ESCAPE as u32,
            Key::Enter => vk = VK_RETURN as u32,
        }
    }

    Ok((modifier, vk))
}

#[cfg(test)]
mod tests {
    use super::*;
    use winapi::um::winuser::VK_F3;
    use winapi::um::winuser::VK_OEM_PERIOD;
    use winapi::um::winuser::VK_SPACE;
    use winapi::um::winuser::VK_TAB;

    #[test]
    fn test_fkey() {
        assert_eq!(fkey("F12".into()), Ok(("".into(), Key::F(12))));
        assert_eq!(fkey("f1".into()), Ok(("".into(), Key::F(1))));
        assert_eq!(fkey("f1 ".into()), Ok((" ".into(), Key::F(1))));

        assert_matches!(fkey("f".into()), Err(Error::Error(_)));
        assert_matches!(fkey("1".into()), Err(Error::Error(_)));
        assert_matches!(fkey("fa".into()), Err(Error::Error(_)));
        assert_matches!(fkey("f 3".into()), Err(Error::Error(_)));
    }

    #[test]
    fn test_modifier() {
        assert_eq!(modifier("Alt".into()), Ok(("".into(), Key::Alt)));
        assert_eq!(modifier("Alt ".into()), Ok((" ".into(), Key::Alt)));
        assert_eq!(modifier("alt".into()), Ok(("".into(), Key::Alt)));
        assert_eq!(modifier("ALT".into()), Ok(("".into(), Key::Alt)));
        assert_eq!(modifier("CTRL".into()), Ok(("".into(), Key::Ctrl)));
        assert_eq!(modifier("WIN".into()), Ok(("".into(), Key::Win)));
        assert_eq!(modifier("SHIFT".into()), Ok(("".into(), Key::Shift)));
    }

    #[test]
    fn test_key() {
        assert_eq!(key("F12".into()), Ok(("".into(), Key::F(12))));
        assert_eq!(key("SHIFT".into()), Ok(("".into(), Key::Shift)));
        assert_eq!(key("SPACE".into()), Ok(("".into(), Key::Char(' '))));
    }

    #[test]
    fn test_key_combi() {
        assert_eq!(key_combo("ALT+F3".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(
            key_combo("WIN + ALT + F3".into()),
            Ok(("".into(), vec![Key::Win, Key::Alt, Key::F(3)])));
        assert_eq!(key_combo(" ALT + F3 ".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(key_combo(" ALT+  F3    ".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(key_combo("SHIFT".into()), Ok(("".into(), vec![Key::Shift])));

        assert_matches!(key_combo("ALT+F 3".into()), Err(Error::Error(_)));
    }

    #[test]
    fn test_to_vk() {
        const ALT: u32 = MOD_ALT as u32;
        const F3: u32 = VK_F3 as u32;
        const A: u32 = 0x41u32;
        const PERIOD: u32 = VK_OEM_PERIOD as u32;

        assert_matches!(to_vk(&vec![Key::Alt, Key::F(3)]), Ok((ALT, F3)));
        assert_matches!(to_vk(&vec![Key::Alt, Key::Char('a')]), Ok((ALT, A)));
        assert_matches!(to_vk(&vec![Key::Alt, Key::Char('.')]), Ok((ALT, PERIOD)));
    }

    #[test]
    fn test_char_to_vk() {
        const SHIFT: u32 = MOD_SHIFT as u32;
        const A: u32 = 0x41u32;
        const SPACE: u32 = VK_SPACE as u32;
        const TAB: u32 = VK_TAB as u32;

        assert_matches!(char_to_vk(&'a'), Ok((0, A)));
        assert_matches!(char_to_vk(&'A'), Ok((SHIFT, A)));
        assert_matches!(char_to_vk(&' '), Ok((0, SPACE)));
        assert_matches!(char_to_vk(&'\t'), Ok((0, TAB)));

        assert_matches!(char_to_vk(&'🤷'), Err(err) => {
            assert!(format!("{:?}", err).contains("two utf16 char"))
        });
        assert_matches!(char_to_vk(&'¥'), Err(err) => {
            assert!(format!("{:?}", err).contains("current input locale"))
        });
    }
}