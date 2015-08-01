/*
 * ipcconnection.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_WINDOWS_IPC_IPCCONNECTION_H_
#define HOOKLIB_WINDOWS_IPC_IPCCONNECTION_H_

#include "../core/memory.h"

namespace Windows {

class IPCData;

class IPCConnection {
public:
  IPCConnection() { }
  IPCConnection(const std::wstring& path);

  IPCConnection(IPCConnection&& other) = default;
  IPCConnection& operator=(IPCConnection&&) = default;

  void callFunction(const std::string& func_name, const IPCData& data);

private:
  HandleEx file_;

  void write(const char* data, std::size_t datalen);
};

} /* namespace Windows */

#endif /* HOOKLIB_WINDOWS_IPC_IPCCONNECTION_H_ */
