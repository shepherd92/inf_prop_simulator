#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

#include "types.h"

#include "logger.h"
#include "network.h"
#include "network_builder.h"
#include "node.h"
#include "constant_degree_distribution.h"
#include "poisson_degree_distribution.h"
#include "power_law_degree_distribution.h"
#include "uniform_degree_distribution.h"

namespace simulator
{

network_builder::network_builder(std::mt19937 &randomNumberGenerator)
   : mRandomNumberGenerator {randomNumberGenerator}
{
}

std::unique_ptr<network>
network_builder::construct(const network_properties &networkProperties)
{
   LOG(DEBUG, "Network build started.");

   node_list nodes {create_nodes(networkProperties)};
   const auto degreeDistribution {create_degree_distribution(networkProperties)};
   const auto connectivity {connect_nodes_randomly(nodes, degreeDistribution, networkProperties.mLoopsOk)};

   std::unique_ptr<network> networkProduct {};

   if(connectivity_result::everything_ok == connectivity || networkProperties.mDanglingConnectionsOk)
   {
      networkProduct = std::make_unique<network>();
      networkProduct->set_nodes(nodes);
      LOG(DEBUG, "Network build finished.");
   }
   else
   {
      LOG(DEBUG, "Built network is not considered due to dangling connections.");
   }

   return networkProduct;
}

node_list network_builder::create_nodes(
   const network_properties &networkProperties) const
{
   node_list nodes {std::vector<std::unique_ptr<node>>(networkProperties.mNumOfNodes)};

   for(node_id nodeId = 0U; nodeId < nodes.size(); ++nodeId)
   {
      nodes[nodeId] = std::make_unique<node>(nodeId, networkProperties.mCharacteristicTime, mRandomNumberGenerator);
   }

   return nodes;
}

std::unique_ptr<int_degree_distribution>
network_builder::create_degree_distribution(
   const network_properties &networkProperties) const
{
   std::unique_ptr<int_degree_distribution> degreeDistribution {nullptr};

   // in order to have a giant component in the network, the Molloy-Reed
   // criterion has to be fulfilled <k^2> - 2 <k> > 0
   // https://en.wikipedia.org/wiki/Configuration_model
   if(degree_distribution_type::constant ==
      networkProperties.mDegreeDistributionType)
   {
      // Molloy-Reed criterion: k_min > 2
      LOG(DEBUG, "Degree distribution is constant.");
      degreeDistribution = std::make_unique<constant_degree_distribution>(
         networkProperties.mKMin);
   }
   else if(degree_distribution_type::uniform ==
      networkProperties.mDegreeDistributionType)
   {
      // Molloy-Reed criterion: a^2 + b^2 + a*b > 3*(a+b)
      const degree kMax = static_cast <degree> (networkProperties.mDegreeDistributionParameter1);
      LOG(DEBUG, "Degree distribution is uniform.");
      degreeDistribution = std::make_unique<uniform_degree_distribution>(
         networkProperties.mKMin, kMax, mRandomNumberGenerator);
   }
   else if(degree_distribution_type::power_law ==
           networkProperties.mDegreeDistributionType)
   {
      // Molloy-Reed criterion: 1 < power < 4
      const auto power = networkProperties.mDegreeDistributionParameter1;
      LOG(DEBUG, "Degree distribution is power law with power.", power);
      degreeDistribution = std::make_unique<power_law_degree_distribution>(
         networkProperties.mKMin, power, networkProperties.mNumOfNodes, mRandomNumberGenerator);
   }
   else if(degree_distribution_type::poisson ==
           networkProperties.mDegreeDistributionType)
   {
      // Molloy-Reed criterion: q * (q-1) > 0; q > 1
      const auto parameter = networkProperties.mDegreeDistributionParameter1;
      LOG(DEBUG, "Degree distribution is Poisson with parameter ", parameter);
      degreeDistribution = std::make_unique<poisson_degree_distribution>(
         networkProperties.mKMin, parameter, networkProperties.mNumOfNodes, mRandomNumberGenerator);
   }
   else
   {
      LOG(CRIT, "Unknown degree distribution type given.");
      assert(false);
   }

   degreeDistribution->generate_distribution();

   return degreeDistribution;
}

connectivity_result network_builder::connect_nodes_randomly(
   const node_list &nodes,
   const std::unique_ptr<int_degree_distribution> &degreeDistribution,
   const bool allowLoops) const
{
   LOG(DEBUG, "Connecting nodes started.");

   // initialize the density function of the free degrees to be connected
   auto freeDegreePdf {create_free_degree_pdf(nodes, degreeDistribution)};

   // create as many random connections in the graph as possible
   while(is_possible_to_create_connection(freeDegreePdf, allowLoops))
   {
      create_random_connection(nodes, freeDegreePdf, allowLoops);
   }

   LOG(DEBUG, "Connecting nodes finished.");

   // it is not possible to create new connections
   // if the freeDegreePdf is not all zero, there are nodes with dangling connections
   // in this case return this information
   const bool connections_full
   {
      std::all_of(freeDegreePdf.begin(),freeDegreePdf.end(),[] (const degree d)
      {
         return 0U == d;
      })
   };
   return connections_full ? connectivity_result::everything_ok : connectivity_result::dangling_connections;
}

std::vector<degree> network_builder::create_free_degree_pdf(
   const node_list &nodes,
   const std::unique_ptr<int_degree_distribution> &degreeDistribution) const
{
   LOG(DEBUG, "Creating node degrees started.");

   // set node degrees randomly
   std::vector<degree> freeDegreePdf {};
   for(auto i = 0U; i < nodes.size(); ++i)
   {
      const degree randomDegree = degreeDistribution->get_random_degree();
      freeDegreePdf.push_back(randomDegree);
   }

   assert(freeDegreePdf.size() == nodes.size());
   LOG(DEBUG, "Creating node degrees finished.");

   return freeDegreePdf;
}

bool network_builder::is_possible_to_create_connection(
   const std::vector<degree> &freeDegreePdf,
   const bool allowLoops) const
{
   return allowLoops
      ? is_possible_to_create_connection_with_loops(freeDegreePdf)
      : is_possible_to_create_connection_without_loops(freeDegreePdf);
}

void network_builder::create_random_connection(
   const node_list &nodes, std::vector<degree> &freeDegreePdf, const bool allowLoops) const
{
   // prerequisite: at least two nodes with free degrees

   // create the two endpoints of the connection
   const node_id firstNodeId {get_node_id_for_random_degree(freeDegreePdf)};
   --freeDegreePdf[firstNodeId];  // the probability density function is updated
   const node_id secondNodeId {get_node_id_for_random_degree(freeDegreePdf)};
   --freeDegreePdf[secondNodeId];  // the probability density function is updated

   if(firstNodeId != secondNodeId || (firstNodeId == secondNodeId && allowLoops))
   {
      // creating a connection between first node and second node
      LOG(DEBUG, "Connecting nodes ", firstNodeId, " and ", secondNodeId);

      // add connections to the nodes
      connection connectionFirstToSecond {connection::type::outgoing, secondNodeId};
      connection connectionSecondToFirst {connection::type::incoming, firstNodeId};
      nodes[firstNodeId]->add_connection(connectionFirstToSecond);
      nodes[secondNodeId]->add_connection(connectionSecondToFirst);
   }
   else
   {
      // the freeDegreePdf has to be reset, as no connection was made
      ++freeDegreePdf[firstNodeId];
      ++freeDegreePdf[secondNodeId];
   }
}

bool network_builder::is_possible_to_create_connection_without_loops(
   const std::vector<degree> &freeDegreePdf) const
{
   // check for at least two distinct, nonzero values in the degree probability
   // density function
   uint32_t numOfNonZeroElements {0U};
   for(const auto pdfValue : freeDegreePdf)
   {
      if(0U != pdfValue)
      {
         ++numOfNonZeroElements;

         if(2U == numOfNonZeroElements)
         {
            return true;
         }
      }
   }
   return false;
}

bool network_builder::is_possible_to_create_connection_with_loops(
   const std::vector<degree> &freeDegreePdf) const
{
   uint32_t numOfFreeDegrees {0U};
   for(auto& freeDegree : freeDegreePdf)
   {
      numOfFreeDegrees += freeDegree;
      if(numOfFreeDegrees >= 2U)
      {
         return true;
      }
   }
   return false;
}

node_id network_builder::get_node_id_for_random_degree(
   const std::vector<degree> &freeDegreePdf) const
{
   // choose a random degree between 0 and the sum of the degrees
   std::uniform_int_distribution<degree> uniformIntDistribution {
      1U, std::accumulate(freeDegreePdf.begin(), freeDegreePdf.end(), 0U)};
   const degree randomDegree {uniformIntDistribution(mRandomNumberGenerator)};

   // choose the node ID which belongs to the randomly chosen degree
   // this is done through calculating the cumulative distribution function
   degree cumulativeSum {0U};
   for(node_id nodeId {0U}; nodeId < freeDegreePdf.size(); ++nodeId)
   {
      cumulativeSum += freeDegreePdf[nodeId];
      if(cumulativeSum >= randomDegree)
      {
         return nodeId;
      }
   }

   // the node ID should have been found
   assert(false && "The node ID is not found.");
   return node_id {};
}

} // namespace simulator

