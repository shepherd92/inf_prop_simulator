#include <cassert>

#include "node.h"

#include "types.h"

namespace simulator {

bool essentially_equal(const double a, const double b) {
  return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) *
                         std::numeric_limits<double>::epsilon());
}

void test_constructor() {
  node testObj(node_id{3});
  assert(testObj.get_degree() == 0);
  assert(testObj.get_node_id() == 3);
  assert(!testObj.is_informed());
}

void test_add_connection() {
  node testObj(node_id{1});

  testObj.add_connection(
      connection{connection::type{connection::type::outgoing}, node_id{2U}});
  testObj.add_connection(
      connection{connection::type{connection::type::incoming}, node_id{6U}});
  testObj.add_connection(
      connection{connection::type{connection::type::incoming}, node_id{7U}});

  assert(testObj.get_degree() == 3);
}

void test_is_connected_to() {
  node testObj(node_id{1});

  testObj.add_connection(
      connection{connection::type{connection::type::outgoing}, node_id{2U}});

  assert(!testObj.is_connected_to(node_id{1U}));
  assert(testObj.is_connected_to(node_id{2U}));
}

void test_inform() {
  // create test object
  node testObj(node_id{1});
  testObj.add_connection(
      connection{connection::type{connection::type::outgoing}, node_id{2U}});
  testObj.add_connection(
      connection{connection::type{connection::type::incoming}, node_id{6U}});
  testObj.add_connection(
      connection{connection::type{connection::type::incoming}, node_id{7U}});

  // call tested method
  assert(!testObj.is_informed());
  event_list newEventList = testObj.inform(time{4.7}, 1.0);
  assert(testObj.is_informed());

  // test information time
  std::optional<time> informationTime = testObj.get_information_time();
  assert(informationTime.has_value());
  assert(essentially_equal(informationTime.value(), 4.7));

  // test new events
  assert(newEventList->size() == 3);
  assert(newEventList->at(0).mTime > 4.7);
  assert(newEventList->at(1).mTime > 4.7);
  assert(newEventList->at(2).mTime > 4.7);
}

} // namespace simulator

int main() {
  using namespace simulator;

  test_constructor();
  test_add_connection();
  test_is_connected_to();
  test_inform();

  return 0;
}
