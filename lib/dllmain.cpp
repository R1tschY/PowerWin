// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.

#include "DesktopHooks.h"
#include "PluginManager.h"
#include "Plugin.h"

// Jedes mal wenn Prozess an Dll geheftet wird aufgerufen
// -> d.h. wenn Hooks laufen jedes mal wenn Prozess gestartet wird
extern "C" BOOL APIENTRY
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  WinExtra::updateDllInstance(hinstDLL);

  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    DisableThreadLibraryCalls(hinstDLL);
    break;

  case DLL_PROCESS_DETACH:
    break;
  }

  return TRUE;
}

