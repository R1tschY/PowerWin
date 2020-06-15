use crate::actions::Actions;
use config::Config;

pub struct ModuleContext<'a> {
    actions: &'a mut Actions,
    config: &'a Config,
}

impl<'a> ModuleContext<'a> {
    pub fn new(actions: &'a mut Actions, config: &'a Config) -> Self {
        ModuleContext { actions, config }
    }

    pub fn actions(&mut self) -> &mut Actions {
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
