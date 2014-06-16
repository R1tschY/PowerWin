#ifndef POINTER_H
#define POINTER_H

#include <cstddef>

namespace Cpp {

namespace Pointer {

template<typename T>
inline void* packFrom(T t) {
  static_assert(sizeof(T) <= sizeof(std::size_t), "type cast fatal because of bit loose");

  union { T o; void* p; } u;
  u.o = t;
  return u.p;
}

template<typename T>
inline T unpackFrom(void* ptr) {
  union { T o; void* p; } u;
  u.p = ptr;
  return u.o;
}

} // namespace Pointer

} // namespace Cpp

#endif // POINTER_H
