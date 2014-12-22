#ifndef UTILWINDOW_H
#define UTILWINDOW_H

#include <windows.h>
#include <functional>

namespace Windows {

typedef std::function<void()> Callback;

#define WM_CALLBACK     (WM_USER + 20)

HWND getCallbackWindow();

} // namespace Windows

#endif // UTILWINDOW_H
