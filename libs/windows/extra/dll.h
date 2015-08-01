#ifndef DLL_H
#define DLL_H

#include <windows.h>

#ifdef BUILD_DLL

namespace Windows {

#define WINDOWS_DLL_SHARED __attribute__((section ("shared"), shared))

extern "C" {
BOOL APIENTRY DllMain(HINSTANCE, DWORD, LPVOID);
}

class Dll {
  Dll();
public:

  static Dll& get();

  static HINSTANCE getInstance() { return get().instance_; }

private:
  HINSTANCE instance_;

  friend BOOL APIENTRY DllMain(HINSTANCE, DWORD, LPVOID);
};

namespace Module {

inline HMODULE getInstance()
{
  return Dll::getInstance();
}

}

} // namespace Windows

#else

#include "../base/application.h"

namespace Windows {
namespace Module {

inline HMODULE getInstance()
{
  return Application::getInstance();
}

}
} // namespace Windows

#endif

#endif // DLL_H
