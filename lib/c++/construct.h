#ifndef CPP_CONSTRUCT_H
#define CPP_CONSTRUCT_H

#include <type_traits>
#include <memory>
#include <utility>
#include <iterator>

#include "macros.h"

namespace cpp {

template<typename T, typename ...Args> inline
void construct(T* ptr, Args&&... args) {
  ::new(ptr) T(std::forward<Args>(args)...);
}

template<typename T, REQUIRES(std::is_trivially_destructible<T>)> inline
void destruct(T*) { }

template<typename T, REQUIRES(!std::is_trivially_destructible<T>)> inline
void destruct(T* ptr) {
  ptr->~T();
}

template<typename ForwardIterator, REQUIRES(std::is_trivially_destructible<typename std::iterator_traits<ForwardIterator>::value_type>)> inline
void destruct(ForwardIterator, ForwardIterator) { }

template<typename ForwardIterator, REQUIRES(!std::is_trivially_destructible<typename std::iterator_traits<ForwardIterator>::value_type>)>
void destruct(ForwardIterator first, ForwardIterator last) {
  for (; first != last; ++first) {
    destruct(std::addressof(*first));
  }
}

template<typename T>
void destruct(T* instance);

template<typename ForwardIterator>
void destruct(ForwardIterator begin, ForwardIterator end);

} // namespace cpp

#endif // CPP_CONSTRUCT_H
