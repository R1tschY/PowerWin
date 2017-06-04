/*
 * toqt.h
 *
 *  Created on: 04.06.2017
 *      Author: Richard
 */

#ifndef APP_HELPER_TOQT_H_
#define APP_HELPER_TOQT_H_

#include <type_traits>
#include <QString>
#include <cpp-utils/strings/string_view.h>

namespace PowerWin {

template<typename WCharArray>
std::enable_if_t<
  std::is_same<WCharArray, const wchar_t*>::value,
  const wchar_t*
>
to(const QString& str)
{
  // TODO: look at qt source code for better way
  return reinterpret_cast<const wchar_t*>(str.utf16());
}

template<typename WStringView>
std::enable_if_t<
  std::is_same<WStringView, cpp::wstring_view>::value,
  cpp::wstring_view
>
to(const QString& str)
{
  // TODO: look at qt source code for better way
  return cpp::wstring_view(
    reinterpret_cast<const wchar_t*>(str.utf16()),
    str.size()
  );
}

}  // namespace PowerWin

#endif /* APP_HELPER_TOQT_H_ */
