#include <cassert>
#include <random>

#include "network_builder.h"
#include "node.h"
#include "types.h"

// undefine LOG macro to not do anything
#undef LOG
#define LOG(level, ...) ;

namespace simulator
{

void test_create_nodes()
{
   network_properties properties {
       10U, 1.5, degree_distribution_type::power_law, 1U, 2.0};
   std::mt19937 randomNumberGenerator {std::random_device{}()};
   network_builder testObj {randomNumberGenerator};
   int_node_list nodes = testObj.create_nodes(properties);
   assert(10U == nodes.size());
}

void test_connect_nodes_randomly()
{
   network_properties properties {
       5U, 1.5, degree_distribution_type::power_law, 4U, 2.0};
   std::mt19937 randomNumberGenerator {std::random_device{}()};
   network_builder testObj {randomNumberGenerator};

   int_node_list nodes = testObj.create_nodes(properties);
   const auto distribution = testObj.create_degree_distribution(properties);
   testObj.connect_nodes_randomly(nodes, distribution, properties.mLoopsOk);

   // at most one node can exist which has different degree than 4
   uint32_t numOfNodesWithDifferentDegree {0u};
   for(auto &node : nodes)
   {
      if(4U != node->get_degree())
      {
         ++numOfNodesWithDifferentDegree;
      }
   }
   assert(numOfNodesWithDifferentDegree <= 1U);
}

} // namespace simulator

int main()
{
   using namespace simulator;

   test_create_nodes();
   // test_connect_nodes_randomly();

   return 0;
}
