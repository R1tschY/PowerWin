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
  constexpr basic_string_view() :
    ref_(nullptr), length_(0)
  { }

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

  constexpr std::basic_string<Char> to_string() const
  {
    return std::basic_string<Char>(ref_, length_);
  }

  constexpr cpp::basic_substring<Char> to_substring() const
  {
    return {cbegin(), cend()};
  }

  constexpr const Char* c_str() const
  {
    return ref_;
  }

  constexpr const Char* data() const
  {
    return ref_;
  }

  constexpr std::size_t size() const { return length_; }
  constexpr std::size_t length() const { return length_; }
  constexpr bool empty() const { return !ref_ || length_ == 0; }
  constexpr bool okay() const { return ref_; }
  constexpr explicit operator bool () const { return ref_; }

  constexpr const Char* begin() const { return ref_; }
  constexpr const Char* end() const { return ref_ + length_; }
  constexpr const Char* cbegin() const { return ref_; }
  constexpr const Char* cend() const { return ref_ + length_; }

  template<typename CharT, std::size_t N>
  void copy_to(CharT (&arg)[N]) {
    static_assert(N > 0, "N > 0");
    assert(!empty());

    std::char_traits<CharT>::copy(arg, data(), N-1);
    arg[N-1] = 0;
  }

  template<typename CharT>
  void copy_to(std::basic_string<CharT>& arg) {
    arg.assign(begin(), end());
  }

private:
  const Char* const ref_;
  const std::size_t length_;

  static constexpr std::size_t length(const Char* const str) {
    return (str) ? traits::length(str) : 0;
  }
};

template<typename CharT>
std::basic_string<CharT>& operator += (std::basic_string<CharT>& lhs, const basic_string_view<CharT> rhs) {
  return lhs.append(rhs.begin(), rhs.end());
}

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;

} // namespace cpp

#endif // CPP_ZSTRING_REF_H
