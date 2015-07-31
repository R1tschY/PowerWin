/*
 * ipcobject.cpp
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#include "ipcmailbox.h"

#include <windows.h>
#include "windows/debug.h"
#include "ipcdata.h"

namespace Windows {

IPCMailbox::IPCMailbox(const std::wstring& path, std::size_t max_data_size, unsigned read_timeout)
{
  mailslot_ = HandleEx(CreateMailslotW(
			path.c_str(),
			max_data_size,
			read_timeout,
			nullptr));
	win_throw_on_fail(mailslot_.get() != INVALID_HANDLE_VALUE);
}

void IPCMailbox::registerFunction(const std::string& func_name, Func func)
{
  vtbl_[func_name] = func;
}

void IPCMailbox::callFunction(const std::string& func_name,
		const IPCData& data)
{
  auto iter = vtbl_.find(func_name);
  if (iter == vtbl_.end())
  {
    print(L"no function: %s", func_name.c_str());
    return;
    //throw std::invalid_argument("func_name");
  }

  iter->second(data);
}

void IPCMailbox::readLoop()
{
  Handle hEvent = Handle(CreateEventW(nullptr, false, false, L"PowerWinMailslot"));
  win_throw_on_fail(hEvent != nullptr);

  OVERLAPPED ov;
  ov.Offset = 0;
  ov.OffsetHigh = 0;
  ov.hEvent = hEvent.get();

  while (true)
  {
    DWORD next_message_size = 0;
    DWORD max_message_size = 0;
    bool success = GetMailslotInfo(
        mailslot_.get(),
        &max_message_size,
        &next_message_size,
        nullptr,
        nullptr);
    win_throw_on_fail(success == true);

    if (next_message_size == MAILSLOT_NO_MESSAGE)
    {
      // wait for next message
      next_message_size = max_message_size;
      continue;
    }

    std::vector<char> data;
    data.resize(next_message_size);

    DWORD bytes_read = 0;
    success = ReadFile(
        mailslot_.get(),
        data.data(),
        next_message_size,
        &bytes_read,
        &ov);
    win_throw_on_fail(success == true);

    if (memcmp(data.data(), "quit", 5) == 0)
    {
      print(L"quit mailslot.");
      break;
    }

    callFunction(std::string(data.data(), data.data() + bytes_read), IPCData());
  }
}

} /* namespace Windows */
