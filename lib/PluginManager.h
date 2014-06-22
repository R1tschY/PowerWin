#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <vector>
#include "windows/debug.h"

class Plugin;

class PluginManager {
public:
  static PluginManager& get();

  void addPlugin(Plugin* plugin);

  ~PluginManager();

  template<typename Func>
  void forEach(Func func) {
    for (Plugin* plugin : plugins_) {
      func(plugin);
    }
  }

private:
  PluginManager();

  std::vector<Plugin*> plugins_;
};

#endif // PLUGINMANAGER_H
