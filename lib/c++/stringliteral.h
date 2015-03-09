#ifndef STRINGLITERAL_H
#define STRINGLITERAL_H

#include <cstddef>
#include <string>

namespace cpp {

template<typename Char>
class basic_string_literal {
public:
  typedef Char char_type;
  typedef std::char_traits<Char> traits;

  typedef Char value_type;
  typedef const Char* iterator;
  typedef const Char* const_iterator;

private:
  typedef basic_string_literal<Char> self_type;

public:
  constexpr basic_string_literal(const Char* str, std::size_t length) :
    ptr_(str), length_(length)
  { }

  constexpr std::size_t size() const { return length_; }
  constexpr const Char* c_str() const { return ptr_; }

private:
  const Char* const ptr_;
  const std::size_t length_;
};

#define _string_literal_II(s) s
#define _string_literal_I(s) _string_literal_II(s)
#define string_literal(s) (cpp::basic_string_literal<char>(_string_literal_I(s), sizeof(_string_literal_I(s))-1))

#define _wstring_literal_II(s) L##s
#define _wstring_literal_I(s) _wstring_literal_II(s)
#define wstring_literal(s) (cpp::basic_string_literal<wchar_t>(_wstring_literal_I(s), sizeof(_wstring_literal_I(s))/sizeof(wchar_t)-1))

} // namespace cpp

#endif // STRINGLITERAL_H
