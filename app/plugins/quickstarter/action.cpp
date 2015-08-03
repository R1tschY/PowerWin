/// \file action.cpp

#include "action.h"

namespace QuickStarter {

Action::Action(const std::wstring& title, const std::wstring& description)
: Match(title, description)
{
}

bool Action::hasInfo(InfoType type) const
{
  return type & Match::Action;
}

Match::InfoData Action::getInfo(InfoType type) const
{
  return InfoData();
}

} // namespace QuickStarter
