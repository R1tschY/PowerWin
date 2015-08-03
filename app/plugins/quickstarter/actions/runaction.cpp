/// \file runaction.cpp

#include "runaction.h"

#include <c++/algorithmex.h>
#include <windows/extra/process.h>

namespace QuickStarter {

RunAction::RunAction()
: Action(L"Run", L"Run application")
{

}

bool RunAction::canHandleMatch(const Match& match) const
{
  return match.hasInfo(Match::Action | Match::Application);
}

void RunAction::execute(const Match& match) const
{
  if (match.hasInfo(Match::Application))
  {
    Match::InfoData data = match.getInfo(Match::Application);

    // search working dir
    auto iter = std::find_if(data.begin(), data.end(), cpp::first_equal(L"working directory"));
    cpp::wstring_view working_directory =
      (iter != data.end()) ? cpp::wstring_view(iter->second) : cpp::wstring_view();

    // TODO show command

    iter = std::find_if(data.begin(), data.end(), cpp::first_equal(L"command line"));
    if (iter == data.end())
    {
      throw std::runtime_error("needed data in match lacks");
    }

    Windows::Process::runCmdln(cpp::wstring_view(iter->second), working_directory);
  }
  else if (match.hasInfo(Match::Action))
  {
    match.execute();
  }
}

} // namespace QuickStarter
