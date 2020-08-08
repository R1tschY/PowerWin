use lightports::sys::dispatch_messages;
use log::LevelFilter;

use simplelog::{ConfigBuilder, SimpleLogger};
use usewin::app::Application;

fn main() {
    SimpleLogger::init(LevelFilter::Trace, ConfigBuilder::new().build()).unwrap();

    let _app = Application::new().expect("Failed to create app");

    dispatch_messages();
}
