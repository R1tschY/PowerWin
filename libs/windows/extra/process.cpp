#include "process.h"

#include "../core/debug.h"

namespace Windows {

Process Process::runCmdln(cpp::wstring_view cmdln, cpp::wstring_view working_directory)
{
  STARTUPINFO si = { };
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdInput = nullptr;
  si.hStdError = nullptr;
  si.hStdOutput = nullptr;

  std::vector<wchar_t> command_line(cmdln.begin(), cmdln.end());
  PROCESS_INFORMATION pi = { };
  BOOL success = CreateProcessW(
    nullptr,
    command_line.data(),
    nullptr,
    nullptr,
    false,
    0,
    nullptr,
    working_directory.c_str(),
    &si,
    &pi);
  win_throw_on_fail(success != true);

  return Process(pi);
}

Process Process::run(
  cpp::wstring_view exe_path,
  cpp::wstring_view args,
  cpp::wstring_view working_directory)
{
  std::wstring cmdln;
  cmdln.reserve(exe_path.size() + args.size() + 3);
  cmdln += '\"';
  cmdln += exe_path;
  cmdln += '\"';
  cmdln += ' ';
  cmdln += args;

  return runCmdln(cmdln, working_directory);
}

} // namespace Windows
