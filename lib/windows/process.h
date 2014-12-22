#ifndef PROCESS_H
#define PROCESS_H

#include <c++/stringview.h>
#include "memory.h"

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

  constexpr bool okay() const { return process_ != nullptr; }
  constexpr explicit operator bool() const { return okay(); }

  static Process startCmdln(cpp::wstring_view cmdln);
  static Process start(cpp::wstring_view exe_path, std::wstring_view args);

private:
  Handle process_;
  Handle thread_;
};

} // namespace Windows

#endif // PROCESS_H
