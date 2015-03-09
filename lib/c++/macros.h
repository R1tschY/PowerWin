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

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define _TO_WIDESTRING(x) L ## x
#define TO_WIDESTRING(x) _TO_WIDESTRING(x)
#define WSTRINGIFY(x) TO_WIDESTRING(_STRINGIFY(x))

#endif // CPP_MACROS_H
