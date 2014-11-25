#ifndef WINSTRING_REF_H
#define WINSTRING_REF_H

#include <memory>

namespace Windows {

class LocalDeleter
{
public:
  inline void operator()(void* ptr)
  {
    LocalFree(ptr);
  }
};

template<typename T>
using LocalPtr = std::unique_ptr<T, LocalDeleter>;

} // namespace Windows

#endif // WINSTRING_REF_H
