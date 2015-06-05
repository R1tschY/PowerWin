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


// see also: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4121.pdf
template<typename Char, std::size_t N>
class basic_static_string {
public:
  typedef Char char_type;
  typedef std::char_traits<Char> traits;

  typedef Char value_type;
  typedef const Char* iterator;
  typedef const Char* const_iterator;

private:
  typedef basic_static_string<Char> self_type;

public:
  constexpr basic_static_string(const Char (&in)[N])
  {
    for (size_t i = 0; i < n; i++)
      data_[i] = in[i];
  }

  constexpr std::size_t size() const { return N; }
  constexpr const Char* c_str() const { return &data_; }

private:
  const Char const data_[N + 1];
};

template<typename Char, std::size_t N, std::size_t M>
constexpr basic_static_string<Char, N+M> operator+(basic_static_string<Char, N> a, basic_static_string<Char, M> b)
{
    basic_static_string<Char, N+M> out;

    for (size_t i = 0; i < a.n-1; i++)
        out.data[i] = a.data[i];

    for (size_t i = 0; i < b.n; i++)
        out.data[a.n-1+i] = b.data[i];

    return out;
}

template<typename Char, std::size_t N>
constexpr basic_static_string<Char, N> make_static _S(const Char (&s)[N])
{
    return basic_static_string<Char, N>(s,n);
}

} // namespace cpp

#endif // STRINGLITERAL_H
