/*
 * ipcobject.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_WINDOWS_IPC_IPCOBJECT_H_
#define HOOKLIB_WINDOWS_IPC_IPCOBJECT_H_

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

namespace Windows {

class IPCData {
public:
  template<typename T>
  const T& get() const
  {
    if (sizeof(T) != data_.size())
    {
      throw std::runtime_error("wrong data size");
    }
    return *reinterpret_cast<const T*>(data_.data());
  }

private:
  std::vector<char> data_;
};

class IPCObject {
public:
  using Func = std::function<void(IPCData)>;

  IPCObject();

  void registerFunction(const std::wstring& func_name);
  void callFunction(const std::wstring& func_name, IPCData data);

  void runLeadLoop();

private:
  std::unordered_map<std::wstring, Func> vtbl_;
  HANDLE mailslot_;
};

} /* namespace Windows */

#endif /* HOOKLIB_WINDOWS_IPC_IPCOBJECT_H_ */
