/// \file comptr.h

#ifndef HOOKLIB_WINDOWS_COM_COMPTR_H_
#define HOOKLIB_WINDOWS_COM_COMPTR_H_

#include <memory>

namespace Windows {

struct COMDeleter {
  template<typename T>
  void operator()(T* ptr) {
    ptr->Release();
  }
};

template<typename T>
using COMPtr = std::unique_ptr<T, COMDeleter>;

} // namespace Windows

#endif /* HOOKLIB_WINDOWS_COM_COMPTR_H_ */
