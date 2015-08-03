/// \file controler.cpp

#include <app/plugins/quickstarter/controller.h>
#include "sources/startmenusource.h"
#include "actions/runaction.h"
#include "query.h"

namespace QuickStarter {

Controller::Controller()
: main_action_(new RunAction())
{
  sources_.emplace_back(new StartMenuSource());
}

std::vector<MatchResult> Controller::search(const std::wstring& query) const
{
  Query q(Query::Categories::All, query);
  std::vector<MatchResult> result;

  for (auto& source : sources_)
  {
    if (source->canHandleQuery(q))
    {
      std::vector<MatchResult> r = source->search(q);
      result.insert(result.end(),
        std::make_move_iterator(r.begin()), std::make_move_iterator(r.end()));
    }
  }
  return result;
}

Controller::~Controller()
{
}

bool Controller::execute(const Match& match) const
{
  if (main_action_->canHandleMatch(match))
  {
    main_action_->execute(match);
    return true;
  }
  return false;
}

} // namespace QuickStarter
