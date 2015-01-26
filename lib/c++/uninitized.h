#ifndef UNINITIZED_H
#define UNINITIZED_H

#include <utility>

#include "macros.h"
#include "construct.h"
#include "storage.h"

namespace cpp {

template<typename T>
class uninitized {
private:
  NOT_COPYABLE(uninitized);

public:
  constexpr uninitized() { }

  template<class... Args>
  void construct(Args&&... args) {
    cpp::construct(getPointer(), std::forward<Args>(args)...);
  }

  storage<T>& operator = (const T& t) {
    get() = t;
    return *this;
  }

  T* operator -> () {
    return getPointer ();
  }

  T& operator * () {
    return get();
  }

  T* operator & () {
    return getPointer();
  }

  T* getPointer() {
    return &cpp::get<T>(data);
  }

  T& get() {
    return cpp::get<T>(data);
  }

private:
  storage<T> data;
};

} // namespace cpp

#endif // UNINITIZED_H
