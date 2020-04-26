#include <cassert>
#include <cmath>
#include <memory>
#include <numeric>

#include "event_queue.h"

namespace simulator {

bool essentially_equal(const double a, const double b) {
  return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) *
                         std::numeric_limits<double>::epsilon());
}

void test_constructor() {
  event_queue testObj(simulator::time{3.7});
  assert(essentially_equal(testObj.get_current_time(), 3.7));
}

void test_update() {
  // create event list
  event_list eventList = std::make_unique<std::vector<event>>();
  eventList->push_back(event{node_id{3}, node_id{5}, time{8.9}});
  eventList->push_back(event{node_id{2}, node_id{0}, time{5.7}});
  eventList->push_back(event{node_id{3}, node_id{9}, time{62.4}});

  // create test object and call tested method
  event_queue testObj(time{0.0});
  testObj.update(std::move(eventList));

  event nextEvent{};
  nextEvent = testObj.next();
  assert(nextEvent.mFromNodeId == 2);
  assert(nextEvent.mToNodeId == 0);
  assert(essentially_equal(nextEvent.mTime, 5.7));

  nextEvent = testObj.next();
  assert(nextEvent.mFromNodeId == 3);
  assert(nextEvent.mToNodeId == 5);
  assert(essentially_equal(nextEvent.mTime, 8.9));

  nextEvent = testObj.next();
  assert(nextEvent.mFromNodeId == 3);
  assert(nextEvent.mToNodeId == 9);
  assert(essentially_equal(nextEvent.mTime, 62.4));
}

void test_is_empty() {
  event_queue testObj(simulator::time{3.7});
  assert(testObj.is_empty());

  event_list eventList = std::make_unique<std::vector<event>>();
  eventList->push_back(event{node_id{3}, node_id{5}, time{8.9}});
  testObj.update(std::move(eventList));

  assert(!testObj.is_empty());
}

} // namespace simulator

int main() {
  using namespace simulator;

  test_constructor();
  test_update();
  test_is_empty();

  return 0;
}
