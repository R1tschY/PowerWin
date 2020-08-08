use crate::core::services::actions::ActionsManager;
use config::Config;

pub struct ModuleContext<'a> {
    actions: &'a mut ActionsManager,
    config: &'a Config,
}

impl<'a> ModuleContext<'a> {
    pub fn new(actions: &'a mut ActionsManager, config: &'a Config) -> Self {
        ModuleContext { actions, config }
    }

    pub fn actions(&mut self) -> &mut ActionsManager {
        self.actions
    }

    pub fn config(&self) -> &Config {
        &self.config
    }
}

pub trait ModuleBuilder {
    fn name(&self) -> &'static str;
    fn build(&self, ctx: &mut ModuleContext) -> Box<dyn Module>;
}

pub trait Module {
    fn name(&self) -> &'static str;
}
