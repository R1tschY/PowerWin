#ifndef STRINGREF_H
#define STRINGREF_H

#include <string>

namespace Cpp {

template<typename CharT>
class GeneralStringRef
{
public:
  constexpr explicit
  GeneralStringRef() { }

  GeneralStringRef(const std::basic_string<CharT>& string) :
    begin_(string.begin()), end_(string.end())
  { }

  GeneralStringRef(const CharT* string) :
    begin_(string), end_(string + strlen(string))
  { }

  const CharT* begin() const {
    return begin_;
  }

  const CharT* end() const {
    return end_;
  }

  const CharT* cbegin() const {
    return begin_;
  }

  const CharT* cend() const {
    return end_;
  }

  std::size_t size() const {
    return end_ - begin_;
  }

private:
  const CharT* begin_;
  const CharT* end_;
};

typedef GeneralStringRef<char> StringRef;
typedef GeneralStringRef<wchar_t> WStringRef;

} // namespace Cpp

#endif // STRINGREF_H
