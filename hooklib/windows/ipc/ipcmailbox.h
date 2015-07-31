/*
 * ipcobject.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_WINDOWS_IPC_IPCMAILBOX_H_
#define HOOKLIB_WINDOWS_IPC_IPCMAILBOX_H_

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <exception>
#include "windows/memory.h"

namespace Windows {

class IPCData;

class IPCMailbox {
public:
  using Func = std::function<void(const IPCData&)>;

  IPCMailbox(const std::wstring& path, std::size_t max_data_size, unsigned read_timeout);

  void registerFunction(const std::string& func_name, Func func);

  void readLoop();

private:
  std::unordered_map<std::string, Func> vtbl_;
  HandleEx mailslot_;

  void callFunction(const std::string& func_name, const IPCData& data);
};

} /* namespace Windows */

#endif /* HOOKLIB_WINDOWS_IPC_IPCMAILBOX_H_ */
