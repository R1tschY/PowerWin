#ifndef STRINGREF_H
#define STRINGREF_H

#include <string>
#include <iterator>
#include <boost/range/iterator_range_core.hpp>

namespace cpp {

template<typename CharT>
class basic_substring : public boost::iterator_range<const CharT*>
{
public:
  typedef boost::iterator_range<const CharT*> base_type;
  typedef CharT char_type;
  typedef std::char_traits<CharT> traits;

  typedef Char value_type;
  typedef const Char* iterator;
  typedef const Char* const_iterator;

private:
  typedef basic_substring<CharT> self_type;

public:
  constexpr explicit
  basic_substring() { }

  constexpr
  basic_substring(const basic_substring& other) :
    base_type(other.begin(), other.end())
  { }

  basic_substring(const std::basic_string<CharT>& string) :
    base_type(string.data(), string.data() + string.length())
  { }

  basic_substring(const CharT* string) :
    base_type(string, string + traits::length(string))
  { }

  basic_substring(const CharT* begin, const CharT* end) :
    base_type(begin, end)
  { }

  template<std::size_t N>
  void copy_to(CharT (&dest)[N]) const {
    traits::copy(dest, base_type::begin(), std::min((std::size_t)base_type::size(), N));
  }

  void copy_to(std::basic_string<CharT>& dest) const {
    dest.assign(base_type::begin(), base_type::end());
  }

  basic_substring<char_type> to_string() const {
    return basic_substring<char_type>(base_type::begin(), base_type::end());
  }
};

template<typename CharT>
std::basic_string<CharT>& operator += (std::basic_string<CharT>& str, const basic_substring<CharT> ref) {
  return str.append(ref.begin(), ref.end());
}

typedef basic_substring<char> substring;
typedef basic_substring<wchar_t> wsubstring;

} // namespace Cpp

#endif // STRINGREF_H
