#ifndef APPLICATION_H
#define APPLICATION_H

#include <windows.h>
#include <functional>
#include <string>

#include "windows.h"

namespace Windows {

class Application {
 public:
  typedef std::function<int()> Callback;

  Application(const wchar_t* name, HINSTANCE instance);
  ~Application();

  int run(const Callback& entry) const;

  static HINSTANCE getInstance() { return instance; }
  static WindowsVersion getWindowsVersion() { return winversion; }
  static const std::wstring& getPath() { return path; }
  static const std::wstring& getName() { return name; }
  // TODO: static string getWindowsVersionString();

  static std::wstring getExeDir();
  static std::wstring getConigDir();

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
