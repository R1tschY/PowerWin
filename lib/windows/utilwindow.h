#ifndef UTILWINDOW_H
#define UTILWINDOW_H

#include <windows.h>

namespace Windows {

#define WM_CALLBACK     (WM_USER + 20)

HWND createUtilWindow(WNDPROC wndproc);

HWND getCallbackWindow();

} // namespace Windows

#endif // UTILWINDOW_H
