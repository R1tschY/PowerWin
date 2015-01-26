#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>
#include <string>
#include <cassert>

#include "macros.h"
#include "memory.h"
#include <c++/stringview.h>
#include <windows/path.h>
#include <windows.h>
#include "version.h"

namespace Windows {

class Application final {
  DISALLOW_COPY_AND_ASSIGN(Application);

public:
  // types
  typedef std::function<int()> ExecuteFunc;

  // ctor
  Application(cpp::wstring_view name, HINSTANCE instance);

  // run
  int run(ExecuteFunc entry);

  // application properties
  static HINSTANCE getInstance() { return self().appinstance_; }
  static cpp::wstring_view getName() { return self().name_; }

  // paths
  static Path getExecutablePath();
  static Path getConigPath();

  // system
  //static WindowsVersion getWindowsVersion() { return winversion; }
  // TODO: static string getWindowsVersionString();
  static bool Is64BitWindows();

  // messages
  static void processMessages();

private:
  Handle mutex_;
  bool is_running_; // TODO: atomic?

  const HINSTANCE appinstance_;
  const std::wstring name_;

  static Application* instance_;

  static Application& self() { assert(instance_ == nullptr); return *instance_; }
};

} // namespace Windows

#endif // APPLICATION_H
