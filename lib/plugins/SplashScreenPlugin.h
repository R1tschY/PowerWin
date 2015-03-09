/*
 * SplashScreenPlugin.h
 *
 *  Created on: 23.02.2015
 *      Author: richard
 */

#ifndef PLUGINS_SPLASHSCREENPLUGIN_H_
#define PLUGINS_SPLASHSCREENPLUGIN_H_

#include "../Plugin.h"
#include "../windows/controls/onscreendisplay.h"

class SplashScreenPlugin: public Plugin {
public:
	SplashScreenPlugin();

  virtual void onActivate(const Options& options);
  virtual void onDeactivate();
	
private:
	Windows::OnScreenDisplay window_;
};

#endif /* PLUGINS_SPLASHSCREENPLUGIN_H_ */
