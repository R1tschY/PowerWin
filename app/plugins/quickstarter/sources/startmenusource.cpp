/// \file startmenusource.cpp

#include "startmenusource.h"

#include <shlobj.h>
#include <windows.h>
#include <string>
#include <vector>
#include <stack>

#include <windows/base/charcodecs.h>
#include <windows/core/debug.h>
#include <windows/shell/shelllink.h>
#include <windows/shell/utils.h>
#include <windows/core.h>
#include <windows/base/path.h>
#include "../matches/applicationmatch.h"
#include "../query.h"

#ifdef SQLITECPP_ENABLE_ASSERT_HANDLER
namespace SQLite
{
// definition of the assertion handler enabled when SQLITECPP_ENABLE_ASSERT_HANDLER is defined in the project (CMakeList.txt)
void assertion_failed(const char* apFile, const long apLine, const char* apFunc, const char* apExpr, const char* apMsg)
{
    // Print a message to the standard error output stream, and abort the program.
    std::cerr << apFile << ":" << apLine << ":" << " error: assertion failed (" << apExpr << ") in " << apFunc << "() with message \"" << apMsg << "\"\n";
    std::abort();
}
}
#endif

namespace QuickStarter {

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
    print(L"Search path: %ls", path.c_str());
    spec = path + L"\\" + mask;
    directories.pop();

    hFind = Windows::FindHandle(FindFirstFileW(spec.c_str(), &ffd));
    if (hFind.get() != INVALID_HANDLE_VALUE)
    {
      print(L"Cannot open path: %ls", spec.c_str());
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

enum StartMenuDatabaseColumns
{
  SMDBC_Name,
  SMDBC_Description,
  SMDBC_Path,
  SMDBC_WorkingDir,
  SMDBC_Origin,
  SMDBC_Arguments,
  SMDBC_ShowCmd,
  SMDBC_IconPath,
  SMDBC_IconId,
};

StartMenuSource::StartMenuSource()
: db_("applications.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE)
{
  db_.exec("DROP TABLE IF EXISTS apps");
  db_.exec(
    "CREATE TABLE apps ("
    "  name        TEXT NOT NULL,"
    "  description TEXT,"
    "  path        TEXT NOT NULL,"
    "  working_dir TEXT,"
    "  origin      TEXT NOT NULL,"
    "  arguments   TEXT,"
    "  show_cmd    INT,"
    "  icon_path   TEXT,"
    "  icon_id     INT" // TODO: icon BLOB
    ")");

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

    SQLite::Statement q(db_,
      "INSERT INTO apps VALUES ("
      "  :name, :description, :path, NULL, :origin, NULL, NULL, NULL, NULL"
      ")");
    q.bind("name", Windows::to_string(file.getBasename()).c_str());
    q.bind("description", Windows::to_string(description).c_str());
    q.bind("path", Windows::to_string(description).c_str());
    q.bind("origin", Windows::to_string(file.toString()).c_str());
    q.exec();
  }
}

bool StartMenuSource::canHandleQuery(const Query& query)
{
  return query.hasCategories(Query::Categories::App); // TODO: db entries > 0
}

std::vector<MatchResult> StartMenuSource::search(const Query& query)
{
  if (query.getSearchString().empty())
    return std::vector<MatchResult>(); // TODO recent things

  std::vector<MatchResult> result;

  SQLite::Statement q(db_, "SELECT * FROM apps WHERE name LIKE '?%'");
  q.bind(1, Windows::to_string(query.getSearchString()));
  while (q.executeStep())
  {
    result.emplace_back(
      std::unique_ptr<ApplicationMatch>(new ApplicationMatch(
        Windows::to_wstring(q.getColumn(SMDBC_Name).getText(nullptr)),
        Windows::to_wstring(q.getColumn(SMDBC_Description).getText("")),
        Windows::to_wstring(q.getColumn(SMDBC_Path).getText(nullptr)),
        Windows::to_wstring(q.getColumn(SMDBC_Arguments).getText("")),
        Windows::to_wstring(q.getColumn(SMDBC_WorkingDir).getText(""))
      )),
      MatchScore::Excellent);
  }

  return result;
}

} // namespace QuickStarter
