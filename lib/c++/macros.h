#ifndef CPP_MACROS_H
#define CPP_MACROS_H

#define NOT_COPYABLE(ClassName) \
  ClassName(const ClassName&) = delete; \
  ClassName& operator=(const ClassName&) = delete

#define COPYABLE(ClassName) \
  constexpr ClassName(const ClassName&) = default; \
  ClassName& operator=(const ClassName&) = default

#define NOT_MOVABLE(ClassName) \
  ClassName(ClassName&&) = delete; \
  ClassName& operator=(ClassName&&) = delete

#define MOVABLE(ClassName) \
  constexpr ClassName(ClassName&&) = default; \
  ClassName& operator=(ClassName&&) = default

#define REQUIRES(...) typename std::enable_if<__VA_ARGS__::value, bool>::type = false
#define ENABLE_IF(...) typename std::enable_if<__VA_ARGS__::value>::type

#define REQUIRES2(...) typename std::enable_if<(__VA_ARGS__), bool>::type = false

#endif // CPP_MACROS_H
