#include "process.h"

namespace Windows {



Process Process::startCmdln(cpp::wstring_view cmdln) {
  STARTUPINFO si = { };
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdInput = nullptr;
  si.hStdError = nullptr;
  si.hStdOutput = nullptr;

  PROCESS_INFORMATION pi = { };
  BOOL success = CreateProcessW(nullptr,
                                cmdln.c_str(),
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

Process Process::start(cpp::wstring_view exe_path, std::wstring_view args) {
  std::wstring cmdln;
  cmdln += '\"';
  cmdln += exe_path;
  cmdln += '\"';
  cmdln += ' ';
  cmdln += args;

  return startCmdln(cmdln);
}

} // namespace Windows
