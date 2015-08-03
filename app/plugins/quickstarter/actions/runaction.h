/// \file runaction.h

#ifndef APP_PLUGINS_QUICKSTARTER_ACTIONS_RUNACTION_H_
#define APP_PLUGINS_QUICKSTARTER_ACTIONS_RUNACTION_H_

#include "../action.h"

namespace QuickStarter {

class RunAction : public Action
{
public:
  RunAction();

  bool canHandleMatch(const Match& match) const override;
  void execute(const Match& match) const override;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_ACTIONS_RUNACTION_H_ */
