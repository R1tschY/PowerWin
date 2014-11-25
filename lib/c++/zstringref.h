#ifndef CPP_ZSTRING_REF_H
#define CPP_ZSTRING_REF_H

#include <string>

#include "stringref.h"

namespace cpp {

template<typename Char>
class basic_zstring_ref {
public:
  typedef Char char_type;
  typedef std::char_traits<Char> traits;

  typedef const Char* value_type;

private:
  typedef basic_string_ref<Char> self_type;

public:
  basic_zstring_ref(const Char* ref) :
    ref_(ref)
  { }

  basic_zstring_ref(Char* ref) :
    ref_(ref)
  { }

  basic_zstring_ref(const std::basic_string<Char>& string) :
    ref_(string.c_str())
  { }

  operator std::basic_string<Char>() const
  {
    return {ref_};
  }

  operator cpp::basic_string_ref<Char>() const
  {
    return {ref_};
  }

  operator const Char*() const
  {
    return ref_;
  }

  std::size_t size() const { return traits<Char>::length(ref_); }
  bool empty() const { return !ref_ || !*ref_; }
  bool okay() const { return ref_; }
  explicit operator bool () const { return ref_; }

  const Char* begin() const { return ref_; }
  end() { return special_end; }
  //end() { return ref_ + size(); }
  const Char* cbegin() const { return ref_; }
  //cend() { return ref_ + size(); }

private:
  const Char* ref_;
};

using zstring_ref = basic_zstring_ref<char>;
using wzstring_ref = basic_zstring_ref<wchar_t>;

} // namespace cpp

#endif // CPP_ZSTRING_REF_H
