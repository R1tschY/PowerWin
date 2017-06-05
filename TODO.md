## System

### 1.0

* add copyright to all files
* use `git describe --tags --match 'v*'` as PowerWin_VERSION
* remove hooklib/macros.h
* try/catch(...) in every CALLBACK-function
* tray-icon menu with interface for modules
 
* http://msdn.microsoft.com/en-us/library/windows/desktop/ms632599%28v=vs.85%29.aspx#destruction
* RegisterClass: Background = GetStockObject(HOLLOW_BRUSH);

### simular software
* http://www.xtrabuttons.com/
* autohotkey
  
## Known Bugs

### system-menu

* Dead-Lock in cv::namedWindow from OpenCV

### scroll

* no scrolling under cursor in Visual Studio
* wrong scrolling under cursor in ConEmu in secound view 
* no scrolling under cursor if Top level window is foreground window
 
## Modules

### system-menu (implemented)

* always on top (implemented)
* Windows 10: move to other viewport (idea)
* eliminate flicker in system menu -> activate double buffering (idea)
* roll-window like in same linux display-managers (idea)
* to background (idea)

### wkill (implemented)

* xkill in windows
  
### wmodify (idea)

* Move windows with Alt + Button1
* system menu with Alt + Button2
* Resize windows with Alt + Button3 (divided in 8 regions)
* Alt + Shift: move window to one of the four edges while moving the mouse
  
### opacity (idea)

* change opacity of windows

### actions (implemented)

* add user-defined user commands (idea)
* list of all hotkeys in a overlay screen (idea)
* list standard windows hotkeys (Win+L, ...) (idea)

### window-debug (idea)

* show information of window under cursor
* impl.
  * on hotkey show osd
  * in 1 secound intervall:
  * get window under cursor
  * show information in osd
  * if cursor in the near if osd: switch position

