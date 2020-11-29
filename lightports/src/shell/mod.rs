pub mod autostart;
mod known_folder;
mod trayicon;

pub use crate::shell::known_folder::get_known_folder_path;
pub use crate::shell::trayicon::{TrayIcon, TrayIconBuilder};
