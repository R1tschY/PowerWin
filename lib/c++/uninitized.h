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
  NOT_MOVABLE(uninitized);

public:
  template<class... Args>
  void construct(Args&&... args) {
    cpp::construct(&data, std::forward<Args>(args)...);
  }

  storage<T>& operator = (const T& t) {
    get(data) = t;
    return *this;
  }

  T& operator -> () {
    return get(&data);
  }

  T& operator * () {
    return get(&data);
  }

  T* operator & () {
    return !get(&data);
  }

private:
  storage<T> data;
};

} // namespace cpp

#endif // UNINITIZED_H
