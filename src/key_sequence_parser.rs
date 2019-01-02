use std::num;

use nom::{is_digit, digit, ErrorKind, Err as Error, space0};
use nom::types::CompleteStr;

#[derive(Eq, PartialEq, Debug, Copy, Clone)]
pub enum Key {
    Ctrl,
    Alt,
    Shift,
    Win,
    F(u8),
    Char(char)
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

named!(key<CompleteStr, Key>,
    alt!(modifier | fkey)
);

named!(key_seq<CompleteStr, Vec<Key> >,
    exact!(separated_nonempty_list_complete!(
        tag!("+"), delimited!(space0, key, space0)
    ))
);

pub fn parse_key_sequence(input: &str) -> Result<Vec<Key>, ()> {
    // TODO: check for multiple same keys
    match key_seq(input.into()) {
        Ok((_, result)) => Ok(result),
        Err(_) => Err(()),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_fkey() {
        assert_eq!(fkey("F12".into()), Ok(("".into(), Key::F(12))));
        assert_eq!(fkey("f1".into()), Ok(("".into(), Key::F(1))));
        assert_eq!(fkey("f1 ".into()), Ok((" ".into(), Key::F(1))));

        assert!(match fkey("f".into()) { Err(Error::Error(_)) => true, _ => false });
        assert!(match fkey("1".into()) { Err(Error::Error(_)) => true, _ => false });
        assert!(match fkey("fa".into()) { Err(Error::Error(_)) => true, _ => false });
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
    }

    #[test]
    fn test_key_seq() {
        assert_eq!(key_seq("ALT+F3".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(
            key_seq("WIN + ALT + F3".into()),
            Ok(("".into(), vec![Key::Win, Key::Alt, Key::F(3)])));
        assert_eq!(key_seq(" ALT + F3 ".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(key_seq(" ALT+  F3    ".into()), Ok(("".into(), vec![Key::Alt, Key::F(3)])));
        assert_eq!(key_seq("SHIFT".into()), Ok(("".into(), vec![Key::Shift])));

        assert!(match key_seq("ALT+F 3".into()) { Err(Error::Error(_)) => true, _ => false });
    }
}