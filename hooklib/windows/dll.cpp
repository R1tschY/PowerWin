#include "dll.h"

namespace Windows {

Dll::Dll() :
  instance_(nullptr)
{
}

Dll& Dll::get()
{
  static Dll instance;
  return instance;
}

extern "C" BOOL APIENTRY
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID) {
  Dll& self = Dll::get();
  if (self.instance_ == nullptr) {
    self.instance_ = hinstDLL;
  }

  if (fdwReason == DLL_PROCESS_ATTACH) {
    // for more performance
    DisableThreadLibraryCalls(hinstDLL);
  }

  return true;
}

} // namespace Windows
