/*
 * SplashScreenPlugin.h
 *
 *  Created on: 23.02.2015
 *      Author: richard
 */

#ifndef PLUGINS_SPLASHSCREENPLUGIN_H_
#define PLUGINS_SPLASHSCREENPLUGIN_H_

#include <lightports/controls/onscreendisplay.h>
#include <lightports/base/timeout.h>
#include <app/module.h>

class SplashScreenPlugin: public PowerWin::Module {
public:
	SplashScreenPlugin(PowerWin::ModuleContext& context);
	~SplashScreenPlugin();

private:
	Windows::OnScreenDisplay window_;
  Windows::Timeout timeout_;

  void onTimeout();
};

#endif /* PLUGINS_SPLASHSCREENPLUGIN_H_ */
