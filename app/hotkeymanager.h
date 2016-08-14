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

#ifndef APP_HOTKEYMANAGER_H_
#define APP_HOTKEYMANAGER_H_

#include <functional>

#include <lightports/extra/hotkey.h>
#include <lightports/extra/shortcut.h>
#include <lightports/controls/messagesink.h>
#include <boost/container/flat_map.hpp>
#include <boost/optional.hpp>

namespace PowerWin {

class HotkeyManager;

class Hotkey
{
public:
  using Callback = std::function<void()>;

  Hotkey(HotkeyManager& manager);
  ~Hotkey();

  Hotkey(const Hotkey&) = delete;
  Hotkey& operator=(const Hotkey&) = delete;

  void setKey(cpp::wstring_view hotkey);
  void setKey(const Windows::ShortCut& hotkey);
  Windows::ShortCut getKey() const;

  void setCallback(Callback callback);

  HotkeyManager& getManager() const { return manager_; }

  static Windows::ShortCut parse(cpp::wstring_view hotkey);

private:
  HotkeyManager& manager_;
  int id_;
};

/// \brief
class HotkeyManager : private Windows::MessageSink
{
public:
  HotkeyManager();

private:
  struct HotkeyData {
    Windows::ShortCut short_cut_;
    Hotkey::Callback callback_;
  };

  boost::container::flat_map<int, HotkeyData> hotkeys_;
  int last_id_ = 0;

  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;

  int registerHotkey();
  void unregisterHotkey(int id);

  void setKey(int id, const Windows::ShortCut& key);
  Windows::ShortCut getKey(int id) const;
  void setCallback(int id, Hotkey::Callback func);

  friend class Hotkey;
};

} // namespace PowerWin

#endif /* APP_HOTKEYMANAGER_H_ */
