use crate::key_combination::{parse_key_combination, Key};
use log::error;
use log::info;
use nom::lib::std::collections::HashMap;
use std::borrow::Cow;
use std::rc::Rc;
use std::{fmt, io};

pub struct Action {
    id: Cow<'static, str>,
    func: Option<Box<dyn Fn()>>,
}

impl Action {
    fn _new(id: Cow<'static, str>, func: Option<Box<dyn Fn()>>) -> Rc<Self> {
        Rc::new(Action { id, func })
    }

    pub fn new(id: impl Into<Cow<'static, str>>, func: Option<Box<dyn Fn()>>) -> Rc<Self> {
        Self::_new(id.into(), func)
    }

    pub fn with_fn(id: impl Into<Cow<'static, str>>, func: impl Fn() + 'static) -> Rc<Self> {
        Self::_new(id.into(), Some(Box::new(func)))
    }

    pub fn with_iofn(
        id: impl Into<Cow<'static, str>>,
        func: impl Fn() -> io::Result<()> + 'static,
    ) -> Rc<Self> {
        let id = id.into();

        Self::_new(
            id.clone(),
            Some(Box::new(move || match func() {
                Ok(_) => (),
                Err(err) => error!("action {} failed: {}", id, err),
            })),
        )
    }

    pub fn id(&self) -> &str {
        &self.id
    }

    pub fn cow_id(&self) -> Cow<'static, str> {
        self.id.clone()
    }

    pub fn is_empty(&self) -> bool {
        self.func.is_none()
    }

    pub fn trigger(&self) {
        if let Some(func) = &self.func {
            info!("Action triggered: {}", self.id);
            func();
        }
    }
}

#[derive(Eq, PartialEq, Hash, Clone)]
pub struct KeySequence {
    keys: Vec<Key>,
}

impl KeySequence {
    pub fn is_empty(&self) -> bool {
        self.keys.is_empty()
    }

    pub fn keys(&self) -> &[Key] {
        &self.keys
    }

    pub fn parse(input: &str) -> io::Result<Self> {
        if let Some(combi) = parse_key_combination(input) {
            Ok(Self { keys: combi })
        } else {
            Err(io::Error::new(
                io::ErrorKind::InvalidInput,
                "invalid key combination",
            ))
        }
    }
}

impl fmt::Display for KeySequence {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let mut first = true;
        for key in &self.keys {
            if first {
                first = false;
            } else {
                f.write_str(" + ")?;
            }

            key.fmt(f)?;
        }
        Ok(())
    }
}

impl fmt::Debug for KeySequence {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.keys.fmt(f)
    }
}

#[derive(Default)]
pub struct KeyBindings {
    bindings: HashMap<KeySequence, Rc<Action>>,
}

impl KeyBindings {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn bindings(&self) -> &HashMap<KeySequence, Rc<Action>> {
        &self.bindings
    }

    pub fn add(&mut self, action: Rc<Action>, keys: KeySequence) {
        self.bindings.insert(keys, action); // TODO: check for conflicts
    }
}
