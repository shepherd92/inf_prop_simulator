#include <cassert>

#include "network_builder.h"
#include "node.h"
#include "types.h"

namespace simulator {

void test_create_nodes() {
  network_properties properties{
      10U, 0.8, degree_distribution_type::power_law, {1U, 100U}, 2.0};
  network_builder testObj{};
  int_node_list nodes = testObj.create_nodes(properties);
  assert(10U == nodes.size());
}

void test_create_degree_distribution() {
  network_properties properties{
      10U, 0.8, degree_distribution_type::power_law, {3U, 3U}, 2.0};
  network_builder testObj{};
  const auto distribution = testObj.create_degree_distribution(properties);
  assert(3U == distribution->get_random_degree());
}

void test_connect_nodes_randomly() {
  network_properties properties{
      5U, 0.8, degree_distribution_type::power_law, {4U, 4U}, 2.0};
  network_builder testObj{};

  int_node_list nodes = testObj.create_nodes(properties);
  const auto distribution = testObj.create_degree_distribution(properties);
  testObj.connect_nodes_randomly(nodes, distribution);

  // at most one node can exist which has different degree than 4
  uint32_t numOfNodesWithDifferentDegree{0u};
  for (auto &node : nodes) {
    if (4U != node->get_degree()) {
      ++numOfNodesWithDifferentDegree;
    }
  }
  assert(numOfNodesWithDifferentDegree <= 1U);
}

} // namespace simulator

int main() {
  using namespace simulator;

  test_create_nodes();
  test_create_degree_distribution();
  test_connect_nodes_randomly();

  return 0;
}
