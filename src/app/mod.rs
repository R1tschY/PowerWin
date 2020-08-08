use std::collections::HashMap;
use std::io;

use config::{Config, ConfigError, Value};
use lightports::app::app_instance;
use lightports::user_control::UserControl;
use lightports::{
    sys::post_quit_message, sys::WindowClass, sys::WindowStyle,
    user_control::UserControlClassBuilderExt,
};
use log::{debug, error, warn};

use crate::app::trayicon::TrayControl;
use crate::core::actions::{Action, KeyBindings, KeySequence};
use crate::core::services::actions::ActionsManager;
use crate::core::services::hotkey::{HotKeyService, HotKeyServiceImpl};
use crate::module::{Module, ModuleContext, ModuleFactory};
use crate::modules::systemactions::SystemActionsModuleFactory;
use crate::modules::wmodify::WModifyModuleFactory;

mod trayicon;

pub struct Application {
    _bindings: KeyBindings,
    _hot_key_service: HotKeyServiceImpl,
    _app_window: UserControl<TrayControl>,
    _modules: Vec<Box<dyn Module>>,
}

impl Application {
    pub fn new() -> io::Result<Self> {
        let mut default_keybindings: HashMap<String, Value> = HashMap::new();
        default_keybindings.insert("usewin.quit".into(), Value::new(None, "ctrl+f12"));
        default_keybindings.insert("window.move".into(), Value::new(None, "ctrl+alt+1"));
        default_keybindings.insert("window.resize".into(), Value::new(None, "ctrl+alt+2"));

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
        let mut modules = vec![];
        modules.push(SystemActionsModuleFactory.build(&mut ctx));
        modules.push(WModifyModuleFactory.build(&mut ctx));

        actions.register(Action::with_fn("usewin.quit", || post_quit_message(0)));

        let bindings = Application::get_keybindings(&mut settings, &mut actions);
        hot_key_service.refresh(&bindings);

        Ok(Self {
            _bindings: bindings,
            _hot_key_service: hot_key_service,
            _app_window: app_window,
            _modules: modules,
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
