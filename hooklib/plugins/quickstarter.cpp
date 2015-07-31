/// \file quickstarter.cpp

#include "quickstarter.h"

//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
#include <shlobj.h>

#include "windows/shell/shelllink.h"
#include "windows/shell/utils.h"
#include "windows/debug.h"
#include "windows/ipc/ipcdata.h"



QuickStarter::QuickStarter()
: Plugin(wstring_literal("quick_starter")),
  mailslot_(LR"#(\\.\mailslot\PowerWin\32\MailSlot)#", 1024, MAILSLOT_WAIT_FOREVER),
  connection_(LR"#(\\.\mailslot\PowerWin\32\MailSlot)#")
{ }

void QuickStarter::onActivate(const Options& options)
{
  thread_ = std::thread([&](){ mailslot_.readLoop(); });
  Sleep(1000);
  connection_.callFunction("QuickStarter::onActivate", Windows::IPCData());
  //updateDatabase();

  std::wstring path;
  Windows::getKnownFolderPath(CSIDL_PROGRAMS, path);
  print(L"Program folder: %ls\n", path);
}

void QuickStarter::onDeactivate()
{
  connection_.callFunction("QuickStarter::onDeactivate", Windows::IPCData());
  connection_.callFunction("quit", Windows::IPCData());
  thread_.join();
}

void QuickStarter::show()
{

}

void QuickStarter::updateDatabase()
{
  updateApplicationDb();
}

void QuickStarter::updateApplicationDb()
{


/*
  namespace fs = boost::filesystem;

  std::wstring path;
  Windows::getKnownFolderPath(CSIDL_PROGRAMS, path);
  //Windows::getKnownFolderPath(FOLDERID_Programs, path);//FOLDERID_CommonStartMenu
  WIN_MESSAGE("Program folder: %ls\n", path);

  fs::recursive_directory_iterator begin{fs::path(path)};
  fs::recursive_directory_iterator end{};
  Windows::ShellLink link;
  for (auto iter = begin; iter != end; ++iter)
  {
#if 0
    WIN_MESSAGE("Entry: %ls\n", iter->path().wstring().c_str());
#else
    link.open(iter->path().wstring());
    std::wstring description;
    link.getDescription(description);
    WIN_MESSAGE(
      "Entry: %ls: %ls\n",
      iter->path().wstring().c_str(),
      description.c_str());
#endif
  }
*/
}
