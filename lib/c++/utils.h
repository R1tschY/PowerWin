#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

namespace cpp {

template <typename T, std::size_t N>
constexpr inline
std::size_t length(const T(&) [N]) {
  return N;
}

template <typename T, std::size_t N>
constexpr inline
std::size_t length(const T& t) {
  return t.size();
}

inline const wchar_t* display_bool(bool b) {
  return b ? L"true" : L"false";
}

///////////////////////////////////////////////////////////////////////////////
// Window Utils

constexpr inline
int high_word(short l) {
  return l << 16;
}

constexpr inline
int low_word(short l) {
  return l;
}

constexpr inline
int dword(short high, short low) {
  return high_word(high) | low_word(low);
}

} // namespace Cpp

#endif // UTILS_H
