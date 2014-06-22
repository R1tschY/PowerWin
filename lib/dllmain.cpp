#include "DesktopHooks.h"
#include "PluginManager.h"
#include "Plugin.h"

HANDLE hMapFile = nullptr;
LPVOID lpvMem = nullptr;

static bool loadSharedMemory() {
  hMapFile = CreateFileMapping(
      (HANDLE)0xFFFFFFFF,
      nullptr,
      PAGE_READWRITE,
      0,
      sizeof(SharedMemory),
      L"PowerWinDllSharedMemory13");
  if (hMapFile == nullptr) {
     return false;
  }

  bool bFirstProcess = (GetLastError() != ERROR_ALREADY_EXISTS);
  if (!bFirstProcess)
  {
     hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,
                                false,
                                L"PowerWinDllSharedMemory13");
     if (hMapFile == nullptr)
        return false;
  }

  lpvMem = MapViewOfFile(
      hMapFile,
      FILE_MAP_ALL_ACCESS,
      0,
      0,
      0);
  if (lpvMem == nullptr)
     return false;

  if (bFirstProcess)
     memset(lpvMem, 0, sizeof(SharedMemory));

  shared_memory = (SharedMemory*)lpvMem;

  return true;
}

static void unloadSharedMemory() {
  UnmapViewOfFile(lpvMem);
  CloseHandle(hMapFile);
}

extern "C" BOOL APIENTRY
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  WinExtra::updateDllInstance(hinstDLL);

  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    DisableThreadLibraryCalls(hinstDLL);

    if (loadSharedMemory() == false)
      return false;
    break;

  case DLL_PROCESS_DETACH:
    unloadSharedMemory();
    break;
  }

  return TRUE;
}

