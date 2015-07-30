/// \file bits.h

#ifndef HOOKLIB_WINDOWS_BITS_H_
#define HOOKLIB_WINDOWS_BITS_H_

namespace Windows {

// word

constexpr uint16_t low_word(uint32_t x) {
  return static_cast<uint16_t>(x);
}

constexpr uint32_t low_word(uint32_t x, uint16_t value) {
  return (x & 0xFFFF0000) | value;
}

constexpr uint16_t high_word(uint32_t x) {
  return static_cast<uint16_t>(x >> 16);
}

constexpr uint32_t high_word(uint32_t x, uint16_t value) {
  return (x & 0x0000FFFF) | (value << 16);
}

constexpr uint32_t dword(uint16_t high, uint16_t low) {
  return (static_cast<uint32_t>(high) << 16) | (low);
}

// byte

constexpr uint8_t low_byte(uint16_t x) {
  return static_cast<uint8_t>(x);
}

constexpr uint16_t low_byte(uint16_t x, uint8_t value) {
  return (x & 0xFF00) | value;
}

constexpr uint8_t high_byte(uint16_t x) {
  return static_cast<uint8_t>(x >> 8);
}

constexpr uint16_t high_byte(uint16_t x, uint8_t value) {
  return (x & 0x00FF) | (value << 8);
}

constexpr uint16_t word(uint8_t high, uint8_t low) {
  return (static_cast<uint16_t>(high) << 8) | (low);
}

} // namespace Windows

#endif /* HOOKLIB_WINDOWS_BITS_H_ */
