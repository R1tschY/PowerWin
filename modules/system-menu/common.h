#ifndef MODULES_SYSTEM_MENU_COMMON_H_
#define MODULES_SYSTEM_MENU_COMMON_H_

namespace SystemMenu {

enum : unsigned {
	AlwaysOnTop = 0x00270,
};

constexpr const wchar_t* NewWindowMessage = L"powerwin.system-menu.new-window";
constexpr const wchar_t* SetTopmostMessage = L"powerwin.system-menu.set-topmost";

} // namespace SystemMenu

#endif /* MODULES_SYSTEM_MENU_COMMON_H_ */
