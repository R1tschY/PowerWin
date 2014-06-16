#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <algorithm>

namespace extra {

template<typename Container, typename T> inline
bool any_of_equal(const Container& container, const T& val) {
  return container.find(val) != container.end();
}

template<typename Container, typename T>
Container& remove(Container& container, const T& val) {
  auto old_end = std::end(container);
  auto new_end = std::remove(std::begin(container), old_end, val);
  container.erase(new_end, old_end);
  return container;
}

template<typename Container, typename Predicate>
Container& remove_if(Container& container, Predicate pred) {
  auto old_end = std::end(container);
  auto new_end = std::remove_if(std::begin(container), old_end, pred);
  container.erase(new_end, old_end);
  return container;
}

} // namespace extra

#endif // ALGORITHM_H
