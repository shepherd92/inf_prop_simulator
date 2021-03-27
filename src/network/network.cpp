#include <algorithm>
#include <limits>
#include <memory>
#include <random>
#include <vector>

#include "network.h"
#include "node.h"

#include "logger.h"
#include "types.h"

namespace simulator
{

network::network() : mNodes {}, mTransmissibility {0.0}
{
}

void network::set_nodes(node_list &nodes)
{
   mNodes = std::move(nodes);
}

void network::set_transmissibility(const double transmissibility)
{
   mTransmissibility = transmissibility;
}

const node_list &network::get_nodes() const
{
   return mNodes;
}

event_list network::inject_information_to_random_node(const time now)
{
   std::random_device randomDevice {};
   std::mt19937 randomNumberGenerator(randomDevice());
   std::uniform_int_distribution<node_id> distribution {0U, static_cast<node_id>(get_num_of_nodes() - 1U)};

   if(0.0 == randomDevice.entropy())
   {
      randomNumberGenerator = std::mt19937 {static_cast<uint32_t>(
         std::chrono::high_resolution_clock::now().time_since_epoch().count())};
   }

   event_list initialEvents {};
   while(true)
   {
      node_id sourceNodeId {distribution(randomNumberGenerator)};

      if(!is_informed(sourceNodeId))
      {
         LOG(DEBUG, "Node ", sourceNodeId, " is initially informed.");
         initialEvents = inform_node(sourceNodeId, now);
         break;
      }
   }

   return initialEvents;
}

event_list network::inform_node(const node_id nodeId, const time now)
{
   LOG(DEBUG, "Inform node ", nodeId, " at time ", now,
       " (degree: ", mNodes[nodeId]->get_degree(), ")");

   event_list events = mNodes[nodeId]->inform(now);

   return events;
}

bool network::is_informed(const node_id nodeId) const
{
   return mNodes[nodeId]->is_informed();
}

size_t network::get_num_of_nodes() const
{
   return mNodes.size();
}

std::unique_ptr<result> network::get_result(const time timeOfInitialization) const
{
   auto simualtionResult = std::make_unique<result>();

   for(const auto &node : mNodes)
   {
      const time adjustedInformationTime {std::max(node->get_information_time() - timeOfInitialization, 0.0)};
      simualtionResult->push_back(
         result_record {adjustedInformationTime, node->get_degree()});
   }

   // partition out nans from the records
   const auto iteratorToFirstNan = std::partition(
      simualtionResult->begin(), simualtionResult->end(),
      [] (const auto &record)
   {
      return !std::isnan(record.mInformationTime);
   });

   // sort only first records with no nans
   std::sort(
      simualtionResult->begin(), iteratorToFirstNan,
      [] (const result_record &first, const result_record &second) -> bool
   {
      return first.mInformationTime < second.mInformationTime;
   });

   return simualtionResult;
}

} // namespace simulator

