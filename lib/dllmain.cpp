#include "DesktopHooks.h"
#include "Plugin.h"

extern "C" BOOL APIENTRY
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  win_updateDllInstance(hinstDLL);

  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    DisableThreadLibraryCalls(hinstDLL);
    break;

  case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}

