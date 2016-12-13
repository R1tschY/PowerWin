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

#include <functional>

#include <lightports/core.h>
#include <lightports/extra/systeminformation.h>
#include <modules/splash-screen/splashscreenplugin.h>

SplashScreenPlugin::SplashScreenPlugin(PowerWin::ModuleContext& context) :
    window_(),
    timeout_(std::bind(&SplashScreenPlugin::onTimeout, this), 3000)
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

SplashScreenPlugin::~SplashScreenPlugin()
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
