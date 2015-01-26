#ifndef WINSTRING_REF_H
#define WINSTRING_REF_H

#include <memory>
#include <type_traits>
#include <windows.h>

namespace Windows {


//
// Local memory

namespace Detail {
struct LocalDeleter {
  void operator()(void* ptr) {
    LocalFree(ptr);
  }
};
} // namespace Detail

template<typename T>
using LocalPtr = std::unique_ptr<T, Detail::LocalDeleter>;

//
// Handle

#define WINDOWS_DEFINE_HANDLE_DELETER(func) \
  template<typename PointerT> \
  struct func##Functor { \
    typedef PointerT pointer; \
    void operator()(PointerT handle) { \
      func(handle); \
    } \
  }

#define WINDOWS_HANDLE_DELETER(func) func##Functor

#define WINDOWS_DEFINE_GENERIC_HANDLE_TYPE(name, deleter) \
  WINDOWS_DEFINE_HANDLE_DELETER(deleter); \
  template<typename T> \
  using name = ::Windows::GeneralHandle<T, WINDOWS_HANDLE_DELETER(deleter)>;

#define WINDOWS_DEFINE_HANDLE_TYPE(name, type, deleter) \
  WINDOWS_DEFINE_HANDLE_DELETER(deleter); \
  using name = ::Windows::GeneralHandle<type, WINDOWS_HANDLE_DELETER(deleter)>;


template<
  typename PointerT,
  template<typename> class DeleterFuncT
>
using GeneralHandle = std::unique_ptr<PointerT, DeleterFuncT<PointerT> >;

WINDOWS_DEFINE_HANDLE_TYPE(Handle, HANDLE, CloseHandle);

} // namespace Windows

#endif // WINSTRING_REF_H
