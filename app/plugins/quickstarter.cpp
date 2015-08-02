/// \file quickstarter.cpp

#include "quickstarter.h"

//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
#include <shlobj.h>
#include <windows.h>
#include <string>
#include <vector>
#include <stack>

#include <windows/shell/shelllink.h>
#include <windows/shell/utils.h>
#include <windows/core.h>
#include <windows/base/path.h>

namespace {

bool isDotName(const wchar_t* path)
{
  return path[0] == '.' && (path[1] == '\0' || (path[1] == '.' && path[2] == '\0'));
}

bool ListFiles(std::wstring path, std::wstring mask, std::vector<Windows::Path>& files)
{
  Windows::FindHandle hFind;
  WIN32_FIND_DATAW ffd;
  std::wstring spec;
  std::stack<std::wstring> directories;

  directories.push(path);

  while (!directories.empty())
  {
    path = directories.top();
    print(L"Search path: %ls", path);
    spec = path + L"\\" + mask;
    directories.pop();

    hFind = Windows::FindHandle(FindFirstFileW(spec.c_str(), &ffd));
    if (hFind.get() != INVALID_HANDLE_VALUE)
    {
      print("Cannot open path: %ls", spec.c_str());
      win_print_fail();
      continue;
    }

    do {
      if (!isDotName(ffd.cFileName))
      {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          directories.push(path + L"\\" + ffd.cFileName);
        }
        else
        {
          files.emplace_back(path + L"\\" + ffd.cFileName);
        }
      }
    } while (FindNextFileW(hFind.get(), &ffd) != 0);

    DWORD error = GetLastError();
    if (error != ERROR_NO_MORE_FILES)
    {
      win_print_on_fail(HRESULT(error));
      return false;
    }
  }

  return true;
}

}

QuickStarter::QuickStarter()
: Plugin(wstring_literal("quick_starter"))
{ }

void QuickStarter::onActivate(const Options& options)
{
  updateDatabase();
}

void QuickStarter::onDeactivate()
{
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
  std::wstring path;
  std::wstring common_path;

  Windows::getKnownFolderPath(CSIDL_PROGRAMS, path);
  print(L"%ls", path.c_str());
  Windows::getKnownFolderPath(CSIDL_COMMON_PROGRAMS, common_path);
  print(L"%ls", common_path.c_str());
  //Windows::getKnownFolderPath(FOLDERID_Programs, path);//FOLDERID_CommonStartMenu

  std::vector<Windows::Path> files;

  ListFiles(path, L"*.lnk", files); // TODO: url, appref-ms
  ListFiles(common_path, L"*.lnk", files);

  Windows::ShellLink link;
  for (auto& file : files)
  {
    link.open(file.toString());
    std::wstring description;
    try {
      link.getDescription(description);
      print(
        L"Entry: %ls: %ls\n",
        file.getBasename().c_str(),
        description.c_str());
    }
    catch (...)
    {
      print(
        L"Entry: %ls (ERROR)\n",
        file.getBasename().c_str());
    }
  }
}
