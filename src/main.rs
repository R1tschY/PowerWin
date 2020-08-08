use lightports::app::app_instance;
use lightports::extra::icon::Icon;
use lightports::extra::resources::ResourceSize;
use lightports::{
    shell::TrayIcon,
    sys::dispatch_messages,
    sys::post_quit_message,
    sys::LParam,
    sys::WindowStyle,
    sys::{LResult, WParam},
    sys::{Window, WindowClass},
    user_control::UserControlClassBuilderExt,
    usr_ctrl::UsrCtrl,
};
use log::{debug, error, warn, LevelFilter};
use winapi::shared::minwindef::HINSTANCE;
use winapi::um::winuser::*;

use config::{Config, ConfigError, Value};
use lightports::user_control::UserControl;
use simplelog::{ConfigBuilder, SimpleLogger};
use std::collections::HashMap;
use std::io;
use usewin::core::actions::{Action, KeyBindings, KeySequence};
use usewin::core::services::actions::ActionsManager;
use usewin::core::services::hotkey::{HotKeyService, HotKeyServiceImpl};
use usewin::hotkeys::SystemActionsModuleBuilder;
use usewin::module::{ModuleBuilder, ModuleContext};
use usewin::resources::POWERWIN_SMALL_ICON;

const MSG_TRAYICON: u32 = WM_USER + 0x27;

struct TrayControl {
    _tray_icon: TrayIcon,
}

impl UsrCtrl for TrayControl {
    type CreateParam = HINSTANCE;

    fn create(hwnd: Window, hinst: &HINSTANCE) -> Self {
        let tray_icon = TrayIcon::build(hwnd, 42)
            .callback_message(MSG_TRAYICON)
            .icon(
                Icon::from_resource_shared(*hinst, POWERWIN_SMALL_ICON, ResourceSize::Small)
                    .expect("app icon could not be loaded"),
            )
            .build()
            .expect("creation of tray icon failed");
        Self {
            _tray_icon: tray_icon,
        }
    }

    fn message(&self, hwnd: Window, msg: u32, w: WParam, l: LParam) -> LResult {
        match msg {
            WM_CLOSE => {
                post_quit_message(0);
            }
            WM_HOTKEY => {
                println!("WM_HOTKEY");
            }
            //
            // MSG_TRAYICON if l.high_word() as u32 == self.tray_icon.id() => {
            //     println!("msg: {:x} x: {} y: {}", l.low_word(), w.get_x(), w.get_y());
            // }
            _ => (),
        }

        hwnd.default_proc(msg, w, l)
    }
}

struct Application {
    _bindings: KeyBindings,
    _hot_key_service: HotKeyServiceImpl,
    _app_window: UserControl<TrayControl>,
}

impl Application {
    pub fn new() -> io::Result<Self> {
        let mut default_keybindings: HashMap<String, Value> = HashMap::new();
        default_keybindings.insert("usewin.quit".into(), Value::new(None, "ctrl+f12"));

        let mut settings = config::Config::default();
        settings
            .set("shortcuts", default_keybindings)
            .expect("Failed to set default key bindings");
        settings.merge(config::File::with_name("Settings")).unwrap();
        debug!(
            "Settings: {:?}",
            settings
                .clone()
                .try_into::<HashMap<String, HashMap<String, String>>>()
                .unwrap()
        );

        let module = app_instance();

        let win_class = WindowClass::build()
            .class_name("UserControl::test_message")
            .module(module)
            .register_user_control::<TrayControl>()
            .expect("Window class could not be created");

        let app_window = win_class
            .build_window()
            .module(module)
            .style(WindowStyle::OVERLAPPEDWINDOW.bits())
            .size(300, 200)
            .pos(200, 200)
            .create(&module)
            .expect("Window could not be created");

        let mut actions = ActionsManager::new();
        let hot_key_service = HotKeyServiceImpl::new()?;
        let mut ctx = ModuleContext::new(&mut actions, &settings);
        let _module = SystemActionsModuleBuilder.build(&mut ctx);

        actions.register(Action::with_fn("usewin.quit", || post_quit_message(0)));

        let bindings = Application::get_keybindings(&mut settings, &mut actions);
        hot_key_service.refresh(&bindings);

        Ok(Self {
            _bindings: bindings,
            _hot_key_service: hot_key_service,
            _app_window: app_window,
        })
    }

    fn get_keybindings(settings: &mut Config, actions: &mut ActionsManager) -> KeyBindings {
        match settings.get_table("shortcuts") {
            Ok(shortcuts) => {
                let mut bindings = KeyBindings::new();
                for (action_id, keycombo) in shortcuts.iter() {
                    if let Ok(keycombo) = keycombo.clone().into_str() {
                        if let Some(action) = actions.get_action(action_id) {
                            if let Ok(keys) = KeySequence::parse(&keycombo) {
                                bindings.add(action, keys);
                            } else {
                                error!(
                                    "Invalid key sequence for action {}, got {}",
                                    action_id, keycombo
                                );
                            }
                        } else {
                            warn!("Unknown action {} in key bindings", action_id);
                        }
                    } else {
                        error!(
                            "Key sequence for action {} must be a string, got {:?}",
                            action_id, keycombo
                        );
                    }
                }
                bindings
            }
            Err(ConfigError::NotFound(_)) => KeyBindings::new(),
            Err(err) => {
                error!("Error in `shortcuts` group: {:?}", err);
                KeyBindings::new()
            }
        }
    }
}

fn main() {
    SimpleLogger::init(LevelFilter::Trace, ConfigBuilder::new().build()).unwrap();

    let _app = Application::new().expect("Failed to create app");

    dispatch_messages();
}
