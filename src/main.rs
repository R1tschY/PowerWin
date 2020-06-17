use lightports::app::app_instance;
use lightports::extra::icon::Icon;
use lightports::extra::resources::ResourceSize;
use lightports::output_debug_string;
use lightports::{
    shell::TrayIcon,
    sys::dispatch_messages,
    sys::post_quit_message,
    sys::LParam,
    sys::WindowFunctions,
    sys::WindowStyle,
    sys::{LResult, WParam},
    sys::{Window, WindowClass},
    user_control::UserControlClassBuilderExt,
    usr_ctrl::UsrCtrl,
};
use log::{debug, LevelFilter};
use winapi::shared::minwindef::HINSTANCE;
use winapi::um::winuser::*;

use simplelog::{ConfigBuilder, SimpleLogger};
use usewin::actions::Actions;
use usewin::hotkeys::HotKeysModuleBuilder;
use usewin::module::{ModuleBuilder, ModuleContext};
use usewin::resources::POWERWIN_SMALL_ICON;

const MSG_TRAYICON: u32 = WM_USER + 0x27;

struct TrayControl {
    _tray_icon: TrayIcon,
}

impl UsrCtrl for TrayControl {
    type CreateParam = HINSTANCE;

    fn create(hwnd: Window, hinst: &HINSTANCE) -> Self {
        let _tray_icon = TrayIcon::build(hwnd, 42)
            .callback_message(MSG_TRAYICON)
            .icon(
                Icon::from_resource_shared(*hinst, POWERWIN_SMALL_ICON, ResourceSize::Small)
                    .expect("app icon could not be loaded"),
            )
            .add()
            .expect("creation of tray icon failed");

        Self { _tray_icon }
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        match msg {
            WM_CLOSE => {
                post_quit_message(0);
            }
            WM_HOTKEY => {
                println!("WM_HOTKEY");
            }

            MSG_TRAYICON if l.high_word() as u32 == self._tray_icon.id() => {
                println!("msg: {:x} x: {} y: {}", l.low_word(), w.get_x(), w.get_y());
            }

            _ => (),
        }

        hwnd.default_proc(msg, w, l)
    }
}

fn main() {
    SimpleLogger::init(LevelFilter::Debug, ConfigBuilder::new().build()).unwrap();

    debug!("Hello, world!");
    output_debug_string("Hello, windows!");

    let mut settings = config::Config::default();
    settings.merge(config::File::with_name("Settings")).unwrap();
    // println!("\n{:?} \n\n-----------",
    //          settings.try_into::<HashMap<String, HashMap<String, String>>>().unwrap());

    let module = app_instance();

    let win_class = WindowClass::build()
        .class_name("UserControl::test_message")
        .module(module)
        .register_user_control::<TrayControl>()
        .expect("Window class could not be created");

    let window = win_class
        .build_window()
        .module(module)
        .style(WindowStyle::OVERLAPPEDWINDOW.bits())
        .size(300, 200)
        .pos(200, 200)
        .create(&module)
        .expect("Window could not be created");

    let mut actions = Actions::new();
    let mut ctx = ModuleContext::new(&mut actions, &settings);
    let _module = HotKeysModuleBuilder.build(&mut ctx);

    actions.dump_actions();

    window.show(SW_SHOW);

    dispatch_messages();
}
