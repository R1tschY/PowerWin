/*
 * SplashScreenPlugin.cpp
 *
 *  Created on: 23.02.2015
 *      Author: richard
 */

#include "SplashScreenPlugin.h"

#include <functional>

#include "../windows/geometry.h"
#include "../windows/systeminformation.h"
#include "../windows/debug.h"

SplashScreenPlugin::SplashScreenPlugin() :
    Plugin(wstring_literal("splash_screen")),
    window_(),
    timeout_(std::bind(&SplashScreenPlugin::onTimeout, this), -1)
{

}

void SplashScreenPlugin::onActivate(const Options& options) {
  auto monitor_info = Windows::Monitor::getPrimary().getInfo();
  auto monitor_rect = monitor_info.getMonitorRect();

  window_.create(nullptr, wstring_literal("SplashScreenPlugin"), Windows::Rectangle::fromCenter(monitor_rect.getCenter(), 500, 100));
  window_.show();

  timeout_.setInterval(3);
}

void SplashScreenPlugin::onDeactivate() {
  window_.destroy();
}

void SplashScreenPlugin::onTimeout()
{
  print(L"SplashScreenPlugin::onTimeout\n");
  window_.hide();
}
