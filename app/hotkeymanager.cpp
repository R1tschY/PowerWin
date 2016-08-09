///
/// Copyright (c) 2016 R1tschY
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to 
/// deal in the Software without restriction, including without limitation the 
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#include "hotkeymanager.h"

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/lexical_cast.hpp>

#include <lightports/core.h>
#include <lightports/core/debugstream.h>
#include <cpp-utils/assert.h>
#include <windows.h>

namespace PowerWin {

Hotkey::~Hotkey()
{
  if (manager_)
    manager_->unregisterHotkey(*this);
}


HotkeyManager::HotkeyManager()
{
  create(L"PowerWin.HotkeyManager");
}

static
int get_function_key(const std::wstring& key) {
  int fkey = 0;
  try {
    fkey = boost::lexical_cast<int>(key.c_str() + 1);
  } catch (const boost::bad_lexical_cast&) {
    return 0;
  }

  return (fkey > 0 && fkey < 25) ? (fkey - 1 + VK_F1) : 0;
}

Windows::ShortCut HotkeyManager::parseHotkey(cpp::wstring_view hotkey)
{
  Windows::ShortCut result;

  std::vector<std::wstring> splited;
  boost::split(splited, hotkey, boost::is_any_of(L"+"), boost::token_compress_on);

  for (std::wstring& key : splited) {
    boost::to_upper(key);

    if (key.size() == 1) {
      if ((key[0] >= L'A' && key[0] <= L'Z') ||
          (key[0] >= L'0' && key[0] <= L'9'))
      {
        result.key = key[0];
        continue;
      } else {
        return Windows::ShortCut();
      }
    }

    if (key[0] == 'F') {
      int fkey = get_function_key(key);
      if (fkey != 0) {
        result.key = fkey;
        continue;
      } else {
        return Windows::ShortCut();
      }
    }
    if (key.compare(L"CTRL") == 0) {
      result.modifiers |= MOD_CONTROL;
    } else if (key.compare(L"ALT") == 0) {
      result.modifiers |= MOD_ALT;
    } else if (key.compare(L"WIN") == 0) {
      result.modifiers |= MOD_WIN;
    } else if (key.compare(L"SHIFT") == 0) {
      result.modifiers |= MOD_SHIFT;
    } else {
      return Windows::ShortCut();
    }
  }

  return result;
}

Hotkey HotkeyManager::registerHotkey(const Windows::ShortCut& keys, Callback func)
{
  cpp_assert(func);

  win_throw_on_fail(
    RegisterHotKey(
        getNativeHandle(),
        ++last_id_, keys.modifiers, keys.key)
  );

  hotkeys_[last_id_] = std::move(func);

  Hotkey result;
  result.manager_ = this;
  result.id_ = last_id_;
  return result;
}

void HotkeyManager::unregisterHotkey(Hotkey& hotkey)
{
  cpp_assert(hotkey.manager_ == this);

  win_throw_on_fail(
    UnregisterHotKey(
      getNativeHandle(),
      hotkey.id_
    )
  );

  hotkey.manager_ = nullptr;

  hotkeys_.erase(hotkey.id_);
}

Hotkey HotkeyManager::registerHotkey(cpp::wstring_view hotkey, Callback func)
{
  auto shortcut = parseHotkey(hotkey);
  if (!shortcut.isValid())
  {
    Windows::DebugOutputStream() << hotkey << L" is not a valid hotkey" << std::endl;

    Hotkey result;
    result.manager_ = nullptr;
    return result;
  }

  return registerHotkey(shortcut, std::move(func));
}

LRESULT HotkeyManager::onMessage(UINT msg, WPARAM wparam,
  LPARAM lparam)
{
  if (msg != WM_HOTKEY)
    return Control::onMessage(msg, wparam, lparam);

  auto iter = hotkeys_.find(wparam);
  if (iter == hotkeys_.end())
  {
    Windows::DebugOutputStream()
      << L"internal error: hotkey with id = " << int(wparam) << " does not exists." << std::endl;
    return 0;
  }

  iter->second();

  return 0;
}

} // namespace PowerWin
