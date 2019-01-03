use crate::module::Module;
use crate::module::ModuleBuilder;
use crate::module::ModuleContext;
use crate::actions::Actions;
use crate::actions::Action;
use crate::key_combination::parse_key_combination_to_vk;
use std::rc::Rc;
use lightports_gui::sys::post_quit_message;
use std::borrow::Cow;

pub struct HotKeysModuleBuilder();

impl ModuleBuilder for HotKeysModuleBuilder {
    fn name(&self) -> &'static str {
        "hotkeys"
    }

    fn build(&self, ctx: &mut ModuleContext) -> Box<Module> {
        Box::new(HotKeysModule::new(ctx.actions()))
    }
}

pub struct HotKeysModule;

impl HotKeysModule {
    pub fn new(actions: &mut Actions) -> Self {
        if let Ok((modifiers, vk)) = parse_key_combination_to_vk("CTRL+F12") {
            actions.set_action(Action {
                id: Cow::from("usewin.quit"),
                modifiers,
                vk,
                func: Rc::new(|| post_quit_message(0))
            });
        }
        HotKeysModule
    }
}

impl Module for HotKeysModule {
    fn name(&self) -> &'static str {
        "hotkeys"
    }
}