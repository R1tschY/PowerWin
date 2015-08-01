#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

namespace cpp {

template <typename T, std::size_t N>
constexpr inline
std::size_t length(const T(&) [N]) {
  return N;
}

template <typename Container, std::size_t N>
constexpr inline
std::size_t length(const Container& t) {
  return t.size();
}

inline const wchar_t* display_bool(bool b) {
  return b ? L"true" : L"false";
}

///////////////////////////////////////////////////////////////////////////////

constexpr inline
short high_word(int l) {
  return (l >> 16) & 0xFFFF;
}

constexpr inline
short low_word(int word) {
  return word & 0xFFFF;
}

constexpr inline
int dword(short high, short low) {
  return (high << 16) | (low);
}

} // namespace Cpp

#endif // UTILS_H
