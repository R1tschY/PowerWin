#include "match.h"

#include <stdexcept>

namespace QuickStarter {

Match::Match(
  const std::wstring& title,
  const std::wstring& description)
: title_(title), description_(description)
{
}

void Match::execute() const
{
  throw std::runtime_error("Match::execute not implemented");
}

} // namespace QuickStarter
