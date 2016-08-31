/*
 * globalevents.cpp
 *
 *  Created on: 30.08.2016
 *      Author: Richard
 */

#include <app/globalevents.h>

namespace PowerWin {

GlobalEvents::GlobalEvents() = default;
GlobalEvents::~GlobalEvents() = default;

cpp::optional<LRESULT> GlobalEvents::handleWindowsMessage(UINT msg,
    WPARAM wparam, LPARAM lparam)
{
  WindowsMessageEvent event(msg, wparam, lparam);
  wm_signal_(event); // TODO: break foreach if result found
  return event.getOptionalResult();
}

} /* namespace PowerWin */
