use std::collections::HashMap;
use std::io;
use std::num;

use lazy_static::lazy_static;
use nom::space0;
use nom::types::CompleteStr;
use winapi::um::winuser::GetKeyboardLayout;
use winapi::um::winuser::MOD_ALT;
use winapi::um::winuser::MOD_CONTROL;
use winapi::um::winuser::MOD_SHIFT;
use winapi::um::winuser::MOD_WIN;
use winapi::um::winuser::VK_BACK;
use winapi::um::winuser::VK_DELETE;
use winapi::um::winuser::VK_END;
use winapi::um::winuser::VK_ESCAPE;
use winapi::um::winuser::VK_F1;
use winapi::um::winuser::VK_HOME;
use winapi::um::winuser::VK_INSERT;
use winapi::um::winuser::VK_NEXT;
use winapi::um::winuser::VK_PRIOR;
use winapi::um::winuser::VK_RETURN;
use winapi::um::winuser::VkKeyScanExW;

lazy_static! {
    static ref SPECIAL_KEYS: HashMap<&'static str, Key> = {
        [
            ("ctrl", Key::Ctrl),
            ("alt", Key::Alt),
            ("win", Key::Win),
            ("shift", Key::Shift),
            ("space", Key::Char(' ')),
            ("tab", Key::Char('\t')),
            ("enter", Key::Enter),
            ("backspace", Key::Backspace),
            ("home", Key::Home),
            ("end", Key::End),
            ("page_up", Key::PageUp),
            ("page_down", Key::PageDown),
            ("esc", Key::Esc),
            ("plus", Key::Char('+')),
        ].iter().cloned().collect()
    };
}

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

fn is_decimal_digit(d: char) -> bool {
    d.is_ascii_digit()
}

fn is_printable(c: char) -> bool {
    !c.is_whitespace() && !c.is_control()
}

fn parse_function_key(number: CompleteStr) -> Result<Key, num::ParseIntError> {
    // TODO: test for <= 24
    Ok(Key::F(u8::from_str_radix(number.as_ref(), 10)?))
}

named!(fkey<CompleteStr, Key>,
    exact!(map_res!(
        preceded!(char!('f'), take_while1!(is_decimal_digit)),
        parse_function_key
    ))
);

fn parse_char_key(input: CompleteStr) -> Option<Key> {
    let mut chars = input.as_ref().chars();
    let c = chars.next().unwrap();
    match chars.next() {
        Some(_) => None,
        None => Some(Key::Char(c))
    }
}

named!(char_key<CompleteStr, Key>,
    map_opt!(exact!(take_while1!(is_printable)), parse_char_key)
);

fn parse_special_key(input: CompleteStr) -> Option<Key> {
    SPECIAL_KEYS.get(input.as_ref()).cloned()
}

named!(special_key<CompleteStr, Key>,
    map_opt!(take_while1!(|_| true), parse_special_key)
);

named!(key<CompleteStr, Key>,
    alt!(special_key | fkey | char_key)
);

fn parse_key(input: CompleteStr) -> Option<Key> {
    key(input).map(|(_, out)| out).ok()
}

named!(key_combo<CompleteStr, Vec<Key> >,
    exact!(separated_nonempty_list_complete!(
        tag!("+"), delimited!(space0, map_opt!(is_not!(" \t+"), parse_key), space0)
    ))
);

pub fn parse_key_combination(input: &str) -> Option<Vec<Key>> {
    // TODO: check for multiple same keys
    // TODO: check for only one key
    key_combo(input.to_lowercase().as_str().into()).map(|(_, out)| out).ok()
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

pub fn parse_key_combination_to_vk(input: &str) -> io::Result<(u32, u32)> {
    if let Some(combi) = parse_key_combination(input) {
        to_vk(&combi)
    } else {
        Err(io::Error::new(io::ErrorKind::InvalidInput, "invalid key combination"))
    }
}

#[cfg(test)]
mod tests {
    use winapi::um::winuser::VK_F3;
    use winapi::um::winuser::VK_OEM_PERIOD;
    use winapi::um::winuser::VK_SPACE;
    use winapi::um::winuser::VK_TAB;

    use super::*;
    use nom::{Err as Error};

    #[test]
    fn test_fkey() {
        assert_eq!(fkey("f12".into()), Ok(("".into(), Key::F(12))));
        assert_eq!(fkey("f1".into()), Ok(("".into(), Key::F(1))));

        assert_matches!(fkey("f".into()), Err(Error::Error(_)));
        assert_matches!(fkey("1".into()), Err(Error::Error(_)));
        assert_matches!(fkey("fa".into()), Err(Error::Error(_)));
        assert_matches!(fkey("f ".into()), Err(Error::Error(_)));
        assert_matches!(fkey("f 3".into()), Err(Error::Error(_)));
        assert_matches!(fkey("fü".into()), Err(Error::Error(_)));
    }

    #[test]
    fn test_parse_special_key() {
        assert_eq!(parse_special_key("alt".into()), Some(Key::Alt));
        assert_eq!(parse_special_key("ctrl".into()), Some(Key::Ctrl));
        assert_eq!(parse_special_key("win".into()), Some(Key::Win));
        assert_eq!(parse_special_key("shift".into()), Some(Key::Shift));
        assert_eq!(parse_special_key("space".into()), Some(Key::Char(' ')));

        assert_eq!(parse_special_key("a".into()), None);
        assert_eq!(parse_special_key("abc".into()), None);
        assert_eq!(parse_special_key("f1".into()), None);
    }

    #[test]
    fn test_char_key() {
        assert_eq!(char_key("f".into()), Ok(("".into(), Key::Char('f'))));
        assert_eq!(char_key("ä".into()), Ok(("".into(), Key::Char('ä'))));
        assert_eq!(char_key(".".into()), Ok(("".into(), Key::Char('.'))));
        assert_eq!(char_key("§".into()), Ok(("".into(), Key::Char('§'))));
        assert_eq!(char_key("🤷".into()), Ok(("".into(), Key::Char('🤷'))));

        assert_matches!(char_key("".into()), Err(Error::Error(_)));
        assert_matches!(char_key("ff".into()), Err(Error::Error(_)));
        assert_matches!(char_key("\t".into()), Err(Error::Error(_)));
        assert_matches!(char_key("\n".into()), Err(Error::Error(_)));
        assert_matches!(char_key("\r".into()), Err(Error::Error(_)));
        assert_matches!(char_key("\0".into()), Err(Error::Error(_)));
        assert_matches!(char_key("\x13".into()), Err(Error::Error(_)));
        assert_matches!(char_key(" ".into()), Err(Error::Error(_)));
    }

    #[test]
    fn test_key() {
        assert_eq!(key("f12".into()), Ok(("".into(), Key::F(12))));
        assert_eq!(key("shift".into()), Ok(("".into(), Key::Shift)));
        assert_eq!(key("space".into()), Ok(("".into(), Key::Char(' '))));

        assert_matches!(key("f3a".into()), Err(Error::Error(_)));
    }

    #[test]
    fn test_key_combi() {
        assert_eq!(key_combo("alt+f3".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(
            key_combo("win + alt + f3".into()),
            Ok(("".into(), vec![Key::Win, Key::Alt, Key::F(3)])));
        assert_eq!(key_combo(" alt + f3 ".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(key_combo(" alt+  f3    ".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(key_combo("shift".into()), Ok(("".into(), vec![Key::Shift])));

        assert_matches!(key_combo("alt+f 3".into()), Err(Error::Error(_)));
    }

    #[test]
    fn test_parse_key_combination() {
        assert_eq!(parse_key_combination("alt+f3"), Some(vec![Key::Alt, Key::F(3)]));
        assert_eq!(parse_key_combination("alt + f3"), Some(vec![Key::Alt, Key::F(3)]));
        assert_eq!(parse_key_combination("ALT+F3"), Some(vec![Key::Alt, Key::F(3)]));
        assert_eq!(parse_key_combination(" Alt   +\tf3 "), Some(vec![Key::Alt, Key::F(3)]));
        assert_eq!(parse_key_combination("WIN+SPACE"), Some(vec![Key::Win, Key::Char(' ')]));
        assert_eq!(parse_key_combination("CTRL+WIN+SHIFT+HOME"),
                   Some(vec![Key::Ctrl, Key::Win, Key::Shift, Key::Home]));
        assert_eq!(parse_key_combination("HOME"), Some(vec![Key::Home]));
        assert_eq!(parse_key_combination("PAGE_UP"), Some(vec![Key::PageUp]));
        assert_eq!(parse_key_combination("Alt+Ä"), Some(vec![Key::Alt, Key::Char('ä')]));

        assert_eq!(parse_key_combination("alt+f 3"), None);
        assert_eq!(parse_key_combination("a lt+f3"), None);
        assert_eq!(parse_key_combination("alt+"), None);
        assert_eq!(parse_key_combination(""), None);
        assert_eq!(parse_key_combination("+"), None);
        assert_eq!(parse_key_combination("alt+abc"), None);
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