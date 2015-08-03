#include "rundll.h"

#include "../core/debug.h"
#include "../base/path.h"

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
    throw std::runtime_error("RunDll64Bit: dll does not exist.\n");
  }

  return Process::runCmdln(std::move(cmdln));
}

} // namespace Windows
