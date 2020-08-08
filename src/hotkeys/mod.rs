use std::io;
use std::mem;
use std::ptr;

use lightports::{result, void_result, WString};
use winapi::um::powrprof::SetSuspendState;
use winapi::um::processthreadsapi::{GetCurrentProcess, OpenProcessToken};
use winapi::um::reason::{SHTDN_REASON_MAJOR_OTHER, SHTDN_REASON_MINOR_OTHER};
use winapi::um::securitybaseapi::AdjustTokenPrivileges;
use winapi::um::winbase::LookupPrivilegeValueW;
use winapi::um::winnt::{
    HANDLE, SE_PRIVILEGE_ENABLED, SE_SHUTDOWN_NAME, TOKEN_ADJUST_PRIVILEGES, TOKEN_PRIVILEGES,
    TOKEN_QUERY,
};
use winapi::um::winuser::{
    ExitWindowsEx, EWX_HYBRID_SHUTDOWN, EWX_LOGOFF, EWX_REBOOT, EWX_SHUTDOWN,
};

use crate::core::actions::Action;
use crate::core::services::actions::ActionsManager;
use crate::module::Module;
use crate::module::ModuleBuilder;
use crate::module::ModuleContext;

pub struct SystemActionsModuleBuilder;

impl ModuleBuilder for SystemActionsModuleBuilder {
    fn name(&self) -> &'static str {
        "hotkeys"
    }

    fn build(&self, ctx: &mut ModuleContext) -> Box<dyn Module> {
        Box::new(SystemActionsModule::new(ctx.actions()))
    }
}

pub struct SystemActionsModule;

impl SystemActionsModule {
    pub fn new(actions: &mut ActionsManager) -> Self {
        actions.register(Action::with_iofn("system.shutdown", shutdown_system));
        actions.register(Action::with_iofn("system.reboot", reboot_system));
        actions.register(Action::with_iofn("system.suspend", suspend_system));
        actions.register(Action::with_iofn("system.hibernate", hibernate_system));
        actions.register(Action::with_iofn("user.logoff", logoff_user));
        Self
    }
}

impl Module for SystemActionsModule {
    fn name(&self) -> &'static str {
        "hotkeys"
    }
}

unsafe fn enable_privilege(privilege: &str) -> io::Result<()> {
    let mut token: HANDLE = ptr::null_mut();
    let mut tkp: TOKEN_PRIVILEGES = mem::zeroed();

    // Get a token for this process.
    result(OpenProcessToken(
        GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
        &mut token,
    ))?;

    // Get the LUID for the shutdown privilege.
    let privilege_name = WString::from_os_str(privilege);
    result(LookupPrivilegeValueW(
        ptr::null_mut(),
        privilege_name.as_ptr(),
        &mut tkp.Privileges[0].Luid,
    ))?;

    tkp.PrivilegeCount = 1; // one privilege to set
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process.
    void_result(AdjustTokenPrivileges(
        token,
        0,
        &mut tkp,
        0,
        ptr::null_mut(),
        ptr::null_mut(),
    ))
}

fn shutdown_system() -> io::Result<()> {
    unsafe {
        enable_privilege(SE_SHUTDOWN_NAME)?;

        void_result(ExitWindowsEx(
            EWX_HYBRID_SHUTDOWN | EWX_SHUTDOWN,
            SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER,
        ))
    }
}

fn reboot_system() -> io::Result<()> {
    unsafe {
        enable_privilege(SE_SHUTDOWN_NAME)?;

        void_result(ExitWindowsEx(
            EWX_REBOOT,
            SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER,
        ))
    }
}

fn logoff_user() -> io::Result<()> {
    unsafe {
        void_result(ExitWindowsEx(
            EWX_LOGOFF,
            SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER,
        ))
    }
}

fn suspend_system() -> io::Result<()> {
    unsafe {
        enable_privilege(SE_SHUTDOWN_NAME)?;
        void_result(SetSuspendState(false as u8, false as u8, false as u8))
    }
}

fn hibernate_system() -> io::Result<()> {
    unsafe {
        enable_privilege(SE_SHUTDOWN_NAME)?;
        void_result(SetSuspendState(true as u8, false as u8, false as u8))
    }
}
