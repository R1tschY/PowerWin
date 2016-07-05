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

#include <lightports/core.h>
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

LRESULT HotkeyManager::onMessage(UINT msg, WPARAM wparam,
  LPARAM lparam)
{
  if (msg != WM_HOTKEY)
    return Control::onMessage(msg, wparam, lparam);

  auto iter = hotkeys_.find(wparam);
  if (iter == hotkeys_.end())
  {
    // TODO: log error
    return 0;
  }

  iter->second();
}

} // namespace PowerWin
