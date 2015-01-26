#include "rundll.h"

#include "debug.h"
#include "path.h"

namespace Windows {

Process RunDll::execute64BitDll(cpp::wstring_view dll_name, cpp::wstring_view entry, cpp::wstring_view cmdln_args)
{
  cpp::wstring_view rundll32_exe = wstring_literal("C:\\Windows\\Sysnative\\rundll32.exe");

  std::wstring cmdln;
  cmdln += rundll32_exe;
  cmdln += L' ';
  cmdln += dll_name;
  cmdln += L',';
  cmdln += entry;
  cmdln += L' ';
  cmdln += cmdln_args;

  print(L"%ls\n", cmdln.c_str());

  if (!Path::exists(dll_name)) {
    print(L"RunDll64Bit: '%ls' does not exist.\n", dll_name.begin());
    return Process();
  }

  return Process::runCmdln(std::move(cmdln));
}

} // namespace Windows
