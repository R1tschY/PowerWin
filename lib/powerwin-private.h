#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

HINSTANCE win_getDllInstance     ();
HWND      win_getMainWindow      ();
void      win_destroy            ();

#ifdef __cplusplus
} // extern "C"
#endif


