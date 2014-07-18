#ifndef ARRAY_REF_H
#define ARRAY_REF_H

#include <iterator>
#include <algorithm>
#include <boost/range/iterator_range_core.hpp>

namespace cpp {

template<typename T>
class array_ref : public boost::iterator_range<const T*> {
private:
  typedef array_ref<T> self_type;

public:
  typedef boost::iterator_range<T> base_type;
  typedef T element_type;

  constexpr explicit
  array_ref() { }

  template<typename Range>
  array_ref(const Range& range) :
    base_type(std::begin(range), std::end(range))
  { }
};

}

#endif // ARRAY_REF_H
