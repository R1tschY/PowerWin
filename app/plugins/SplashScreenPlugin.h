/*
 * SplashScreenPlugin.h
 *
 *  Created on: 23.02.2015
 *      Author: richard
 */

#ifndef PLUGINS_SPLASHSCREENPLUGIN_H_
#define PLUGINS_SPLASHSCREENPLUGIN_H_

#include "../plugin.h"
#include <windows/controls/onscreendisplay.h>
#include <windows/base/timeout.h>

class SplashScreenPlugin: public Plugin {
public:
	SplashScreenPlugin();

  void onActivate(const Options& options) override;
  void onDeactivate() override;
	
private:
	Windows::OnScreenDisplay window_;
  Windows::Timeout timeout_;

  void onTimeout();
};

#endif /* PLUGINS_SPLASHSCREENPLUGIN_H_ */
