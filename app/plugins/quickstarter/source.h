/// \file source.h

#ifndef APP_PLUGINS_QUICKSTARTER_SOURCES_SOURCE_H_
#define APP_PLUGINS_QUICKSTARTER_SOURCES_SOURCE_H_

#include <vector>
#include <memory>
#include <utility>

namespace QuickStarter {

class Query;
class Match;

namespace MatchScore {
enum MatchScore
{
  IncrementMinor = 2000,
  IncrementSmall = 5000,
  IncrementMedium = 10000,
  IncrementLarge = 20000,

  Poor = 50000,
  BelowAverage = 60000,
  Average = 70000,
  AboveAverage = 75000,
  Good = 80000,
  VeryGood = 85000,
  Excellent = 90000,

  Highest = 100000
};
}

using MatchResult = std::pair<std::unique_ptr<Match>, MatchScore::MatchScore>;

/// \brief
class Source
{
public:
  virtual ~Source() = default;

  virtual bool canHandleQuery(const Query& query) = 0;
  virtual std::vector<MatchResult> search(const Query& query) = 0;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_SOURCE_H_ */
