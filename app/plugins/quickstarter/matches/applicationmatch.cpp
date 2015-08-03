/// \file applicationmatch.cpp

#include <app/plugins/quickstarter/matches/applicationmatch.h>

namespace QuickStarter {

ApplicationMatch::ApplicationMatch(
  const std::wstring& title,
  const std::wstring& description,
  const std::wstring& file,
  const std::wstring& command_line,
  const std::wstring& working_directory)
: Match(title, description),
  file_(file),
  command_line_(command_line),
  working_directory_(working_directory)
{
}

bool ApplicationMatch::hasInfo(InfoType type) const
{
  return type & (Match::Application | Match::File);
}

Match::InfoData ApplicationMatch::getInfo(InfoType type) const
{
  Match::InfoData result;
  if (type & Match::Application)
  {
    if (!working_directory_.empty())
      result.emplace_back(L"working directory", working_directory_);

    result.emplace_back(L"command line",
      std::wstring(L"\"") + file_ + std::wstring(L"\" ") + command_line_);
  }

  if (type & Match::File)
  {
    result.emplace_back(L"file path", file_);
  }

  return result;
}

} // namespace QuickStarter
