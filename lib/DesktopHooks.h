
#include "stdafx.h"
#include "export.h"

#include <windows.h>
#include <functional>

namespace WinExtra {

DLL_PUBLIC HINSTANCE getDllInstance     ();
DLL_PUBLIC void      updateDllInstance  (HINSTANCE instance);
DLL_PUBLIC void      updateMainWindow   (HWND window);
DLL_PUBLIC HWND      getMainWindow      ();
DLL_PUBLIC void      start              ();
DLL_PUBLIC void      destroy            ();

DLL_PUBLIC int       run               (HINSTANCE hInstance, std::function<int()> callback);

} //  namespace WinExtra


