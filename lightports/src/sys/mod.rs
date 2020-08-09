mod messages;
mod window;
mod windowclass;

pub use crate::def::atom::{atom_to_wchar_ptr, AtomOrString};
pub use crate::def::param::{LParam, LResult, WParam};
pub use crate::sys::messages::{dispatch_messages, post_quit_message, send_message};
pub use crate::sys::window::{
    AsHwnd, IsA, Window, WindowBuilder, WindowExStyle, WindowFunctions, WindowStyle,
};
pub use crate::sys::windowclass::{WindowClass, WindowClassBuilder, WindowClassStyle};
