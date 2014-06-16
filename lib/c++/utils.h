#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

namespace Cpp {

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
int toHighWord(short l) {
  return l << 16;
}

constexpr inline
int toLowWord(short l) {
  return l;
}

constexpr inline
int toDword(short high, short low) {
  return toHighWord(high) | toLowWord(low);
}

} // namespace Cpp

#endif // UTILS_H
