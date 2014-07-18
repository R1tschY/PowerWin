#ifndef STRINGREF_H
#define STRINGREF_H

#include <string>
#include <iterator>
#include <boost/range/iterator_range_core.hpp>

namespace cpp {

template<typename CharT>
class basic_string_ref : public boost::iterator_range<const CharT*>
{
public:
  typedef boost::iterator_range<const CharT*> base_type;
  typedef CharT char_type;
  typedef std::char_traits<CharT> traits;

  typedef const CharT* value_type;

private:
  typedef basic_string_ref<CharT> self_type;

public:
  constexpr explicit
  basic_string_ref() { }

  constexpr
  basic_string_ref(const basic_string_ref& other) :
    base_type(other.begin(), other.end())
  { }

  basic_string_ref(const std::basic_string<CharT>& string) :
    base_type(string.data(), string.data() + string.length())
  { }

  basic_string_ref(const CharT* string) :
    base_type(string, string + traits::length(string))
  { }

  basic_string_ref(const CharT* begin, const CharT* end) :
    base_type(begin, end)
  { }

  template<std::size_t N>
  void copy_to(CharT (&dest)[N]) const {
    traits::copy(dest, base_type::begin(), std::min((std::size_t)base_type::size(), N));
  }

  void copy_to(std::basic_string<CharT>& dest) const {
    dest.assign(base_type::begin(), base_type::end());
  }

  basic_string_ref<char_type> to_string() const {
    return basic_string_ref<char_type>(base_type::begin(), base_type::end());
  }
};

template<typename CharT>
std::basic_string<CharT>& operator += (std::basic_string<CharT>& str, const basic_string_ref<CharT> ref) {
	return str.append(ref.begin(), ref.end());
}

typedef basic_string_ref<char> string_ref;
typedef basic_string_ref<wchar_t> wstring_ref;

} // namespace Cpp

#endif // STRINGREF_H
