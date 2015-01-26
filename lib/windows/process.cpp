#include "process.h"

namespace Windows {

Process Process::runCmdln(std::wstring cmdln) {
  STARTUPINFO si = { };
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdInput = nullptr;
  si.hStdError = nullptr;
  si.hStdOutput = nullptr;

  PROCESS_INFORMATION pi = { };
  BOOL success = CreateProcessW(nullptr,
                                const_cast<wchar_t*>(cmdln.c_str()),
                                nullptr,
                                nullptr,
                                false,
                                0,
                                nullptr,
                                nullptr,
                                &si,
                                &pi);
  if (success) {
    return Process(pi);
  }
  else
  {
    return Process();
  }
}

Process Process::run(cpp::wstring_view exe_path, cpp::wstring_view args) {
  std::wstring cmdln;
  cmdln += '\"';
  cmdln += exe_path;
  cmdln += '\"';
  cmdln += ' ';
  cmdln += args;

  return runCmdln(std::move(cmdln));
}

} // namespace Windows
