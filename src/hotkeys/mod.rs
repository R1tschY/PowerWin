use std::io;
use std::mem;
use std::ptr;
use std::rc::Rc;

use lightports::result;
use lightports::sys::post_quit_message;
use lightports::WString;
use winapi::um::processthreadsapi::GetCurrentProcess;
use winapi::um::processthreadsapi::OpenProcessToken;
use winapi::um::reason::SHTDN_REASON_MAJOR_OTHER;
use winapi::um::reason::SHTDN_REASON_MINOR_OTHER;
use winapi::um::securitybaseapi::AdjustTokenPrivileges;
use winapi::um::winbase::LookupPrivilegeValueW;
use winapi::um::winnt::HANDLE;
use winapi::um::winnt::SE_PRIVILEGE_ENABLED;
use winapi::um::winnt::SE_SHUTDOWN_NAME;
use winapi::um::winnt::TOKEN_ADJUST_PRIVILEGES;
use winapi::um::winnt::TOKEN_PRIVILEGES;
use winapi::um::winnt::TOKEN_QUERY;
use winapi::um::winuser::ExitWindowsEx;
use winapi::um::winuser::EWX_HYBRID_SHUTDOWN;
use winapi::um::winuser::EWX_SHUTDOWN;

use crate::actions::Actions;
use crate::module::Module;
use crate::module::ModuleBuilder;
use crate::module::ModuleContext;
use winapi::um::winuser::EWX_LOGOFF;
use winapi::um::winuser::EWX_REBOOT;

pub struct HotKeysModuleBuilder();

impl ModuleBuilder for HotKeysModuleBuilder {
    fn name(&self) -> &'static str {
        "hotkeys"
    }

    fn build(&self, ctx: &mut ModuleContext) -> Box<dyn Module> {
        Box::new(HotKeysModule::new(ctx.actions()))
    }
}

pub struct HotKeysModule;

impl HotKeysModule {
    pub fn new(actions: &mut Actions) -> Self {
        actions.set_action("usewin.quit", "CTRL+F12", Rc::new(|| post_quit_message(0)));
        actions.set_system_action("system.shutdown", "CTRL+F11", shutdown_system);
        actions.set_system_action("system.reboot", "CTRL+F10", reboot_system);
        actions.set_system_action("user.logoff", "CTRL+F9", logoff_user);
        HotKeysModule
    }
}

impl Module for HotKeysModule {
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
    let privilege_name = WString::from_str(privilege);
    result(LookupPrivilegeValueW(
        ptr::null_mut(),
        privilege_name.as_ptr(),
        &mut tkp.Privileges[0].Luid,
    ))?;

    tkp.PrivilegeCount = 1; // one privilege to set
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process.
    result(AdjustTokenPrivileges(
        token,
        0,
        &mut tkp,
        0,
        ptr::null_mut(),
        ptr::null_mut(),
    ))?;

    Ok(())
}

fn shutdown_system() -> io::Result<()> {
    unsafe {
        enable_privilege(SE_SHUTDOWN_NAME)?;

        result(ExitWindowsEx(
            EWX_HYBRID_SHUTDOWN | EWX_SHUTDOWN,
            SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER,
        ))?;
    }

    Ok(())
}

fn reboot_system() -> io::Result<()> {
    unsafe {
        enable_privilege(SE_SHUTDOWN_NAME)?;

        result(ExitWindowsEx(
            EWX_REBOOT,
            SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER,
        ))?;
    }

    Ok(())
}

fn logoff_user() -> io::Result<()> {
    unsafe {
        result(ExitWindowsEx(
            EWX_LOGOFF,
            SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER,
        ))?;
    }

    Ok(())
}
