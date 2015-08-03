/// \file algorithmex.h

#ifndef LIBS_C___ALGORITHMEX_H_
#define LIBS_C___ALGORITHMEX_H_

namespace cpp {

template<typename T>
struct _first_equal
{
  _first_equal(const T& t_) : t(t_) { }

  template<typename U>
  bool operator()(const U& u) const
  {
    return t == u.first;
  }

private:
  const T& t;
};

template<typename T>
_first_equal<T> first_equal(const T& t)
{
  return _first_equal<T>(t);
}

} // namespace cpp

#endif /* LIBS_C___ALGORITHMEX_H_ */
