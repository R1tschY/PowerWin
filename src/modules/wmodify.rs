use lightports::sys::{Window, WindowFunctions};
use winapi::shared::minwindef::WPARAM;
use winapi::um::winuser::{
    SC_CLOSE, SC_CONTEXTHELP, SC_KEYMENU, SC_MAXIMIZE, SC_MINIMIZE, SC_MOVE, SC_NEXTWINDOW,
    SC_PREVWINDOW, SC_RESTORE, SC_SIZE, WM_SYSCOMMAND,
};

use crate::core::actions::Action;
use crate::core::services::actions::ActionsManager;
use crate::module::{Module, ModuleContext, ModuleFactory};
use std::io;
use std::rc::Rc;

pub struct WModifyModuleFactory;

impl ModuleFactory for WModifyModuleFactory {
    fn name(&self) -> &'static str {
        "wmodify"
    }

    fn build(&self, ctx: &mut ModuleContext) -> Box<dyn Module> {
        Box::new(WModifyModule::new(ctx.actions()))
    }
}

pub struct WModifyModule;

impl WModifyModule {
    pub fn new(actions: &mut ActionsManager) -> Self {
        actions.register(system_command_action("window.move", SC_MOVE));
        actions.register(system_command_action("window.resize", SC_SIZE));
        actions.register(system_command_action("window.maximize", SC_MAXIMIZE)); // WIN+UP
        actions.register(system_command_action("window.minimize", SC_MINIMIZE)); // WIN+DOWN
        actions.register(system_command_action("window.restore", SC_RESTORE)); // WIN+DOWN
        actions.register(system_command_action("window.next", SC_NEXTWINDOW));
        actions.register(system_command_action("window.previous", SC_PREVWINDOW));
        actions.register(system_command_action("window.contexthelp", SC_CONTEXTHELP));
        actions.register(system_command_action("window.menu", SC_KEYMENU)); // ALT+SPACE
        actions.register(system_command_action("window.close", SC_CLOSE)); // ALT+F4
        Self
    }
}

impl Module for WModifyModule {
    fn name(&self) -> &'static str {
        "wmodify"
    }
}

fn system_command(msg: usize) -> io::Result<()> {
    if let Some(foreground_window) = Window::get_foreground_window() {
        foreground_window.post_message(WM_SYSCOMMAND, msg.into(), 0.into())
    } else {
        Ok(())
    }
}

fn system_command_action(id: &'static str, msg: usize) -> Rc<Action> {
    Action::with_iofn(id, || system_command(msg))
}
