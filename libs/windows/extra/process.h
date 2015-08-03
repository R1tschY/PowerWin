#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <c++/stringview.h>
#include "../core/memory.h"
#include <windows.h>

namespace Windows {

class Process
{
public:
  explicit Process(const PROCESS_INFORMATION& pi) :
    process_(pi.hProcess), thread_(pi.hThread)
  { }

  static Process runCmdln(
    cpp::wstring_view cmdln,
    cpp::wstring_view working_directory = cpp::wstring_view());

  static Process run(
    cpp::wstring_view exe_path,
    cpp::wstring_view args,
    cpp::wstring_view working_directory = cpp::wstring_view());

private:
  Handle process_;
  Handle thread_;
};

} // namespace Windows

#endif // PROCESS_H
