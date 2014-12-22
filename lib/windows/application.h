#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <functional>
#include <string>

#include "windows.h"
#include "debug.h"
#include <c++/stringref.h>

namespace Windows {

class Application {
 public:
  typedef std::function<int()> Callback;

  Application(cpp::wstring_view name, HINSTANCE instance);
  ~Application();

  int run(const Callback& entry) const;

  static HINSTANCE getInstance() {
    if (instance == 0) {
      print(L"too early access of app instance!\n");
    }
    return instance;
  }
  static WindowsVersion getWindowsVersion() { return winversion; }
  static bool Is64BitWindows();
  static const std::wstring& getPath() { return path; }
  static const std::wstring& getName() { return name; }
  // TODO: static string getWindowsVersionString();

  static std::wstring getExecutableFilename();
  static std::wstring getExecutablePath();
  static std::wstring getConigPath();

private:
  HANDLE mutex_;
  bool is_running_;

  static HINSTANCE instance;
  static WindowsVersion winversion;
  static std::wstring path;
  static std::wstring name;
};

} // namespace Windows

#endif // APPLICATION_H
