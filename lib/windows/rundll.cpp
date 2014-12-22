#include "rundll.h"

namespace Windows {

Process RunDll::execute64BitDll(cpp::wsubstring dll_name, cpp::wsubstring entry, cpp::wsubstring cmdln_args)
{
  cpp::wsubstring rundll32_exe = lit("C:\\Windows\\Sysnative\\rundll32.exe");

  std::wstring cmdln;
  cmdln += rundll32_exe;
  cmdln += L' ';
  cmdln += dll_name;
  cmdln += L',';
  cmdln += entry;
  cmdln += L' ';
  cmdln += cmdln_args;

  print(L"%ls\n", cmdln.c_str());

  if (!Windows::PathExists(dll_name)) {
    print(L"RunDll64Bit: '%ls' does not exist.\n", dll_name.begin());
    return false;
  }

  return RunCmdln(std::move(cmdln));
}

} // namespace Windows
