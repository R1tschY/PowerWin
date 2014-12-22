#ifndef CPP_ZSTRING_REF_H
#define CPP_ZSTRING_REF_H

#include <string>
#include "substring.h"
#include "stringliteral.h"

namespace cpp {

template<typename Char>
class basic_string_view {
public:
  typedef Char char_type;
  typedef std::char_traits<Char> traits;

  typedef Char value_type;
  typedef const Char* iterator;
  typedef const Char* const_iterator;

private:
  typedef basic_string_view<Char> self_type;

public:
  constexpr basic_string_view(const Char* ref) :
    ref_(ref), length_(length(ref))
  { }

  constexpr basic_string_view(const std::basic_string<Char>& string) :
    ref_(string.c_str()), length_(string.size())
  { }

  constexpr basic_string_view(const cpp::basic_string_literal<Char>& string) :
    ref_(string.c_str()), length_(string.size())
  { }

  constexpr operator std::basic_string<Char>() const
  {
    return std::basic_string<Char>(ref_, length_);
  }

  constexpr operator cpp::basic_substring<Char>() const
  {
    return {cbegin(), cend()};
  }

  constexpr operator const Char*() const
  {
    return ref_;
  }

  constexpr std::size_t size() const { return length_; }
  constexpr bool empty() const { return !ref_ || length_ == 0; }
  constexpr bool okay() const { return ref_; }
  constexpr explicit operator bool () const { return ref_; }

  constexpr const Char* begin() const { return ref_; }
  constexpr end() { return ref_ + size(); }
  constexpr const Char* cbegin() const { return ref_; }
  constexpr cend() { return ref_ + size(); }

private:
  const Char* const ref_;
  const std::size_t length_;

  static constexpr std::size_t length(const Char* const str) {
    return traits<Char>::length(str);
  }
};

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;

} // namespace cpp

#endif // CPP_ZSTRING_REF_H
