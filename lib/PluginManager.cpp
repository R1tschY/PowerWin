#include "PluginManager.h"

#include "macros.h"
#include "Plugin.h"

PluginManager::PluginManager() :
  plugins_()
{
  plugins_.reserve(8);
}

PluginManager::~PluginManager()
{
}

PluginManager& PluginManager::get() {
  static PluginManager instance;
  return instance;
}

void PluginManager::addPlugin(Plugin* plugin)
{
  plugins_.push_back(plugin);
}
