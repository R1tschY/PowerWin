/// \file quickstarter.cpp

#include "quickstarter/controller.h"
#include "quickstarter/match.h"
#include "quickstarter.h"

//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
#include <windows/core.h>

QuickStarterPlugin::QuickStarterPlugin()
: Plugin(wstring_literal("quick_starter"))
{ }

void QuickStarterPlugin::onActivate(const Options& options)
{
  QuickStarter::Controller controller;
  auto results = controller.search(L"Un");
  for (auto& result : results)
  {
    print(L"Result %ls", result.first->getTitle().c_str());
  }
}

void QuickStarterPlugin::onDeactivate()
{
}

void QuickStarterPlugin::show()
{

}

void QuickStarterPlugin::updateDatabase()
{
  updateApplicationDb();
}

void QuickStarterPlugin::updateApplicationDb()
{

}
