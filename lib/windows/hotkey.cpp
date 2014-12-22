#include "hotkey.h"
#include "../windows/utilwindow.h"
#include "../windows/debug.h"

namespace Windows {

namespace {

class HotkeyManager final {
  HotkeyManager();

public:
  HotkeyManager& get();

  static unsigned getUniqueId() { return get().getUniqueId(); }
  unsigned getUniqueId() {
    return counter_++;
  }

  static bool registerHotkey() { return get().registerHotkey(); }
  bool registerHotkey(ShortCut shortcut, HotkeyHandler handler);
  static bool unregisterHotkey() { return get().unregisterHotkey(); }
  bool unregisterHotkey(const Hotkey& hotkey);

private:
  MessageSink message_sink_;

  std::map<unsigned, HotkeyHandler> handlers_;
  unsigned counter_;

  LRESULT HotkeyManager::WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};

HotkeyManager::HotkeyManager() :
  message_sink_(WndProc)
{
  create();
}

HotkeyManager& HotkeyManager::get()
{
  static HotkeyManager hotkey_manager;
  return hotkey_manager;
}

bool HotkeyManager::registerHotkey(const Hotkey& hotkey)
{
  bool success = RegisterHotkey(message_sink_.getNativeHandle(), hotkey.id_, hotkey.modifiers_, hotkey.vk_);
  if (success) {
    handlers_[hotkey.id_] = hotkey.handler_;
  }
  return success;
}

bool HotkeyManager::unregisterHotkey(const Hotkey& hotkey)
{
  handlers_.erase(hotkey.id_);
  return UnregisterHotkey(message_sink_.getNativeHandle(), hotkey.id_);
}

LRESULT HotkeyManager::WndProc(UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_HOTKEY: {
    auto func = get().handlers_.find(wparam);
    if (func != get().handlers_.end()) {
      (*func)();
    }
    return 0;
  }
  }

  return Control::onMessage(msg, wparam, lparam);
}

} // namespace

Hotkey::Hotkey(unsigned modifiers, unsigned vk, Handler handler) :
  modifiers_(modifiers), vk_(vk), handler_(handler), active_(false),
  id_(HotkeyManager::getUniqueId())
{ }

Hotkey::Hotkey(Hotkey&& other) noexcept :
  modifiers_(other.modifiers_), vk_(other.vk_), handler_(other.handler_),
  active_(other.active_), id_(other.id_)
{
  other.active_ = false;
}

Hotkey::~Hotkey() {
  deactivate();
}

bool Hotkey::activate() {
  if (!active_) {
    bool success = HotkeyManager::registerHotkey(*this);
    if (!success) {
      //CRITICAL(L"RegisterHotKey failed: %s", GetLastWindowsError().c_str());
      return false;
    }
    active_ = true;
  }
  return true;
}

bool Hotkey::deactivate() {
  if (active_ && HotkeyManager::unregisterHotkey(*this)) {
    active_ = false;
  }
  return true;
}

} // namespace Windows


