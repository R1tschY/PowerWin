
#pragma once

namespace PowerWin {

enum WKillMouseControlMsg : unsigned {
  Start = 0,
  Stop = 1
};

constexpr const wchar_t* WKillMouseControl = L"powerwin.wkill.mouse-control";

}  // namespace PowerWin
