#ifndef UNINITIZED_H
#define UNINITIZED_H

#include <utility>

#include "macros.h"
#include "construct.h"
#include "storage.h"

namespace cpp {

template<typename T>
class uninitialized {
private:
  NOT_COPYABLE(uninitized);

public:
  constexpr uninitialized() { }

  explicit uninitialized(const uninitialized& other) {
    construct(other);
  }

  explicit uninitialized(uninitialized&& other) {
    construct(std::move(other));
  }

  template<class... Args>
  explicit uninitialized(Args&&... args) { 
    construct(std::forward<Args>(args)...);
  }

  template<class... Args>
  void construct(Args&&... args) {
    ::new(getPointer()) T (std::forward<Args>(args)...);
  }

  uninitialized& operator = (const T& t) { 
    get() = t;
    return *this;
  }

  uninitialized& operator = (T&& t) {
    get() = std::move(t);
    return *this;
  }

  T* operator -> () { return getPointer(); }    
  T& operator * () { return get(); }    
  T* operator & () { return getPointer(); }    
  T* getPointer() { return reinterpret_cast<T*>(&data); }    
  T& get() { return *reinterpret_cast<T*>(&data); }
  
  const T* operator -> () const { return getPointer(); }    
  const T& operator * () const { return get(); }    
  const T* operator & () const { return getPointer(); }    
  const T* getPointer() const { return reinterpret_cast<const T*>(&data); }    
  const T& get() const { return *reinterpret_cast<const T*>(&data); }

private:
  storage<T> data;
};

} // namespace cpp

#endif // UNINITIZED_H
