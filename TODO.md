## System

* i18n with boost::local
* Action backend: powerwin.exit(), windows.start("cmd.exe"), ... with lua
* LuaBrigde
* class HotkeyManager { registerFunction(module, hotkey, name. desciption, lua_code); };
* Log-Backend and log-frontend module

* Lua functions (idea)
  * powerwin.exit() (idea)
  * system.execute(cmdln) (idea)
  * powerwin.osd.show(system.date.format('%Y.%m.%d')..' '..system.time.format('%H:%M:%S'), 'position: center; width:200px; height:200px')
  
* KeepTheCarRunning -> PatchWindows
* tray-icon menu with interface for modules
* http://msdn.microsoft.com/en-us/library/windows/desktop/ms632599%28v=vs.85%29.aspx#destruction
* AnimateWindow (http://msdn.microsoft.com/en-us/library/windows/desktop/ms632669%28v=vs.85%29.aspx)
* RegisterClass: Background = GetStockObject(HOLLOW_BRUSH);

### simular software
* http://www.xtrabuttons.com/
* autohotkey
  
## Bugs

### system-menu

* Dead-Lock in cv::namedWindow from OpenCV

### scroll

* no scrolling on left monitor of main monitor
 
## Modules

### system-menu (implemented)

* always on top (implemented)
* Windows 10: move to other viewport (idea)
* eliminate flicker in system menu -> activate double buffering (idea)

### wkill (idea)

* xkill in windows
* Implementation idea:
  * Registery Hotkey (wkill.start())
  * on hotkey register mouse hook to filter next mouse click
  * on mouse click find top-level window von cursor position
  * call TerminateProcess with window
  
### wmodify (idea)

* Move windows with Alt + Button1
* system menu with Alt + Button2
* Resize windows with Alt + Button3
  
### opacity (idea)

* change opacity of windows

### SheetCheat (idea)

* list of all hotkeys in a overlay screen

### actions (implemented)

* add user-defined user commands (idea)

### log (idea)

### roll-window (idea)

* roll-window like in same linux display-managers
* add system-menu if module is loaded

### window-debug (idea)

* show information of window under cursor
* impl.
  * on hotkey show osd
  * in 1 secound intervall:
  * get window under cursor
  * show information in osd
  * if cursor in the near if osd: switch position if osd

