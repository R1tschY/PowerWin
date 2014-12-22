#ifndef WINSTRING_REF_H
#define WINSTRING_REF_H

#include <windows.h>

#include <memory>
#include <type_traits>

namespace Windows {


//
// Local memory

namespace Detail {
class LocalDeleter
{
public:
  inline void operator()(void* ptr)
  {
    LocalFree(ptr);
  }
};
} // namespace detail

template<typename T>
using LocalPtr = std::unique_ptr<T, Detail::LocalDeleter>;

//
// Handle

namespace Detail {
class HandleDeleter
{
public:
  typedef HANDLE pointer;

  void operator()(HANDLE ptr)
  {
    CloseHandle(ptr);
  }
};
} // namespace detail

template<typename T>
using Handle = std::unique_ptr<HANDLE, Detail::HandleDeleter>;


} // namespace Windows

#endif // WINSTRING_REF_H
