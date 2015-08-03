/// \file action.h

#ifndef APP_PLUGINS_QUICKSTARTER_SOURCES_ACTION_H_
#define APP_PLUGINS_QUICKSTARTER_SOURCES_ACTION_H_

#include "match.h"

namespace QuickStarter {

class Action : public Match
{
public:
  Action(const std::wstring& title, const std::wstring& description);

  virtual bool canHandleMatch(const Match& match) const = 0;
  virtual void execute(const Match& match) const = 0;

  bool hasInfo(InfoType type) const override;
  InfoData getInfo(InfoType type) const override;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_ACTION_H_ */
