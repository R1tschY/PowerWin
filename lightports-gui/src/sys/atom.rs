use std::ffi::c_void;

use lightports::{WString};


/// parameter type for functions, that accept a ATOM or a string.
pub enum AtomOrString {
    Atom(u16),
    Str(WString)
}

impl AtomOrString {
    pub fn as_ptr(&self) -> *const u16 {
        match self {
            AtomOrString::Atom(atom) => atom_to_wchar_ptr(*atom),
            AtomOrString::Str(ref s) => s.as_ptr()
        }
    }
}

/// MAKEINTATOM implementation
pub fn atom_to_wchar_ptr(atom: u16) -> *const u16 {
    atom as usize as *const c_void as *const u16
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::ptr;

    #[test]
    fn test_atom_to_wchar_ptr() {
        assert_eq!(atom_to_wchar_ptr(0), ptr::null());
        assert_eq!(format!("{:?}", atom_to_wchar_ptr(0x3039)), "0x3039");
    }
}