// DesktopHooks.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"

#include <string>
#include <algorithm>
#include <boost/range/algorithm/equal.hpp>

#include "algorithm.h"
#include "DesktopHooks.h"
#include "Hook.h"
#include "ConfigFile.h"
#include "plugins/ActionsPlugin.h"
#include "plugins/ScrollPlugin.h"
#include "plugins/FullscreenPlugin.h"
#include "plugins/SystemMenuPlugin.h"
#include "Utils.h"
#include "PluginManager.h"
#include "windows/application.h"
#include "windows/trayicon.h"

HINSTANCE instance_ = nullptr;
HWND window_ = nullptr;

struct {
  ActionsPlugin actions;
  ScrollPlugin scroll;
//  FullscreenPlugin fullscreen;
  SystemMenuPlugin systemmenu;
} plugins DLL_SHARED;

Windows::TrayIcon tray_icon;

namespace WinExtra {

DLL_PUBLIC
HINSTANCE getDllInstance() {
  return instance_;
}

void updateDllInstance(HINSTANCE instance) {
  if (instance_ == NULL) {
    instance_ = instance;
  }
}

void
updateMainWindow(HWND window) {
  window_ = window;
}

HWND
getMainWindow() {
  return window_;
}

void
start() {
  ConfigFile config;
  config.loadFromFile(Windows::Application::getExeDir());

  PluginManager::get().forEach([&](Plugin* plugin) {
    Plugin::Options opts;  
    for (const std::wstring& key : config.getKeys(plugin->getName())) {
      opts[key] = config.getString(plugin->getName(), key.c_str(), nullptr);
    }
    plugin->setOptions(std::move(opts));

    if (plugin->getBooleanOption(L"active", true)) {
      print(L"activate plugin %s\n", plugin->getName());
      plugin->activate();
    }
  });

  tray_icon.add(getMainWindow(),
                LoadIcon(Windows::Application::getInstance(),
                         MAKEINTRESOURCE(108)));
}

void
destroy() {
  try {
    PluginManager::get().forEach([&](Plugin* plugin) {
      plugin->deactivate();
    });
  } catch (...) {
    MessageBox(NULL, L"Ups", L"!!!", MB_ICONERROR | MB_OK);
  }

  tray_icon.remove();
}

int run(HINSTANCE hInstance, std::function<int()> callback) {
  Windows::Application app(L"PowerWin", hInstance);
  return app.run(callback);
}

} //  namespace WinExtra


///////////////////////////////////////////////////////////////////////////////
// Windowpicker

//auto windowpicker = MouseClickHook([](unsigned button, POINT pt) -> bool {
//  if (button == WM_LBUTTONDOWN) return true;
//  if (button != WM_LBUTTONUP) return false;
  
//  OutputDebugString(L"windowpicker\n");

//  HWND window = WindowFromPoint(pt);
//  if (window == NULL) {
//    OutputDebugString(L"Kein Fenster ausgewählt!\n");
//    return false;
//  } else {
//    wchar_t class_name[255];
//    GetClassNameW(window, class_name, sizeof(class_name));

//    OutputDebugString(L"Fensterklassenname: '");
//    OutputDebugString(class_name);
//    OutputDebugString(L"'\n");
//  }

//  return true;
//});

