/// \file oleptr.h

#ifndef HOOKLIB_WINDOWS_COM_OLEPTR_H_
#define HOOKLIB_WINDOWS_COM_OLEPTR_H_

#include <memory>

namespace Windows {

struct OLEDeleter {
  template<typename T>
  void operator()(T* ptr) {
    CoTaskMemFree(ptr);
  }
};

template<typename T>
using OLEPtr = std::unique_ptr<T, OLEDeleter>;

} // namespace Windows

#endif /* HOOKLIB_WINDOWS_COM_OLEPTR_H_ */
