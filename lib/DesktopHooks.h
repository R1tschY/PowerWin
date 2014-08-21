#include "export.h"

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

HINSTANCE win_getDllInstance     ();
void      win_updateDllInstance  (HINSTANCE instance);
HWND      win_getMainWindow      ();
void      win_destroy            ();

DLL_PUBLIC
void CALLBACK KeepTheCarRunning(HINSTANCE hInstance,
                                HINSTANCE hPrevInstance,
                                LPSTR lpCmdLine,
                                int nCmdShow);

#ifdef __cplusplus
} // extern "C"
#endif


