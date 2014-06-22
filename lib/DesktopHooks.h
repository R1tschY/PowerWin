#include "export.h"

#include <windows.h>
#include <functional>

typedef struct {
  HHOOK systemmenu_hook;
} SharedMemory;

extern SharedMemory* shared_memory;

namespace WinExtra {

HINSTANCE getDllInstance     ();
void      updateDllInstance  (HINSTANCE instance);
HWND      getMainWindow      ();
void      destroy            ();

DLL_PUBLIC int       run               (HINSTANCE hInstance);


} //  namespace WinExtra


