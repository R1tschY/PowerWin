/// \file controler.h

#ifndef APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_
#define APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_

#include <vector>
#include <memory>

#include "source.h"

namespace QuickStarter {

class Source;
class Action;

class Controller
{
public:
  Controller();
  ~Controller();

  std::vector<MatchResult> search(const std::wstring& query) const;
  bool execute(const Match& match) const;

private:
  std::vector<std::unique_ptr<Source>> sources_;
  std::unique_ptr<Action> main_action_;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_CONTROLLER_H_ */
