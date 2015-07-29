#ifndef PROCESS_H
#define PROCESS_H

#include <c++/stringview.h>
#include "memory.h"
#include <windows.h>

namespace Windows {

class Process
{
public:
  constexpr explicit Process() :
    process_(nullptr), thread_(nullptr)
  { }

  explicit Process(const PROCESS_INFORMATION& pi) :
    process_(pi.hProcess), thread_(pi.hThread)
  { }

  bool okay() const { return bool(process_); }
  explicit operator bool() const { return okay(); }

  static Process runCmdln(std::wstring cmdln);

  static Process runCmdln(cpp::wstring_view cmdln) {
    return runCmdln(cmdln.to_string());
  }

  static Process run(cpp::wstring_view exe_path, cpp::wstring_view args);

private:
  Handle process_;
  Handle thread_;
};

} // namespace Windows

#endif // PROCESS_H
