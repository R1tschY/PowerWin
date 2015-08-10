/*
 * resources.cpp
 *
 *  Created on: 09.08.2015
 *      Author: Richard
 */

#include "resources.h"
#include <windows/core/debug.h>
#include <Commctrl.h>

namespace Windows {

std::wstring Resources::getString(HINSTANCE hinstance, unsigned id)
{
  const wchar_t* p = nullptr;
  ::SetLastError(0);
  int len = ::LoadStringW(hinstance, id, reinterpret_cast<LPWSTR>(&p), 0);
  if (len <= 0)
  {
    win_throw_on_fail((HRESULT)::GetLastError());
    return std::wstring();
  }

  return std::wstring(p, static_cast<size_t>(len));
}

HICON Resources::getIcon(HINSTANCE hinstance, int id)
{
  return win_throw_on_fail(LoadIcon(hinstance, MAKEINTRESOURCE(id)));
}

HICON Resources::getIcon(HINSTANCE hinstance, int id, unsigned size)
{
  return (HICON)win_throw_on_fail(LoadImage(hinstance, MAKEINTRESOURCE(id), IMAGE_ICON, size, size, LR_SHARED));
}

HICON Resources::getIconMetric(HINSTANCE hinstance, int id)
{
/*  HICON hicon;
  win_throw_on_fail(LoadIconMetric(hinstance, MAKEINTRESOURCE(id), LIM_SMALL, &hicon));
  return hicon;*/
  return getIcon(hinstance, id);
}


} /* namespace Windows */

