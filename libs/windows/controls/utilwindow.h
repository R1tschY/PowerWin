#ifndef UTILWINDOW_H
#define UTILWINDOW_H

#include <functional>
#include <windows.h>

namespace Windows {

typedef std::function<void()> Callback;

#define WM_CALLBACK     (WM_USER + 20)

HWND getCallbackWindow();

} // namespace Windows

#endif // UTILWINDOW_H
