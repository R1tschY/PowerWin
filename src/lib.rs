extern crate config;
extern crate lightports;
extern crate winapi;
extern crate lazy_static;
#[macro_use] extern crate nom;
#[macro_use] extern crate log;
#[cfg(test)] #[macro_use] extern crate assert_matches;

pub mod actions;
pub mod module;
pub mod key_combination;
pub mod module_manager;
pub mod resources;

pub mod hotkeys;
