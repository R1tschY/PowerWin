#include "PluginManager.h"

#include "extra.h"
#include "Plugin.h"

PluginManager::PluginManager() :
  plugins_()
{
  plugins_.reserve(8);
}

PluginManager& PluginManager::get() {
  static PluginManager instance DLL_SHARED;
  return instance;
}

void PluginManager::addPlugin(Plugin* plugin)
{
  plugins_.push_back(plugin);
}
