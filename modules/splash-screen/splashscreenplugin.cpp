/*
 * SplashScreenPlugin.cpp
 *
 *  Created on: 23.02.2015
 *      Author: richard
 */

#include <functional>

#include <lightports/core.h>
#include <lightports/extra/systeminformation.h>
#include <modules/splash-screen/splashscreenplugin.h>

SplashScreenPlugin::SplashScreenPlugin() :
    window_(),
    timeout_(std::bind(&SplashScreenPlugin::onTimeout, this), 3000)
{ }

void SplashScreenPlugin::activate(PowerWin::ModuleContext& context)
{
  auto monitor_info = Windows::Monitor::getPrimary().getInfo();
  auto monitor_rect = monitor_info.getMonitorRect();

  window_.create(
    nullptr,
    wstring_literal("SplashScreenPlugin"),
    Windows::Rectangle::fromCenter(monitor_rect.getCenter(), 500, 100));
  window_.show();

  timeout_.start();
}

void SplashScreenPlugin::deactivate()
{
  window_.destroy();
  timeout_.stop();
}

void SplashScreenPlugin::onTimeout()
{
  window_.hide();
}

PowerWin::ModuleRegistry::element<SplashScreenPlugin> SplashScreenModule(
  L"splash-screen", L"show splash screen at start of PowerWin"
);
