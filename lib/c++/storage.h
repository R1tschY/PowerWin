#ifndef CPP_STORAGE_H
#define CPP_STORAGE_H

#include <type_traits>

namespace cpp {

template<typename T>
using storage = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;

template<typename T>
T& get(storage<T>& s) {
  return *static_cast<T*>(static_cast<void*>(s));
}

} // namespace cpp

#endif // CPP_STORAGE_H
