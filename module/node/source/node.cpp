#include <chrono>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

#include <iostream>

#include "types.h"

#include "node.h"

#include "int_degree_distribution.h"
#include "logger.h"

namespace simulator
{

node::node(const node_id nodeId,
           const double characteristicTime,
           std::mt19937 &randomNumberGenerator)
   : mNodeId {nodeId}
   , mCharacteristicTime {characteristicTime}
   , mRandomNumberGenerator {randomNumberGenerator}
{
}

void node::add_connection(const connection &connection)
{
   mConnectionList.push_back(connection);
}

bool node::is_connected_to(const node_id &nodeId) const
{
   for(const auto &connection : mConnectionList)
   {
      if(nodeId == connection.mToNodeId)
      {
         return true;
      }
   }
   return false;
}

event_list node::inform(const time now)
{
   LOG(DEBUG, "Node ", mNodeId, " informed at time ", now);

   mIsInformed = true;
   mInformationTime = now;

   event_list newEventList {std::make_unique<std::vector<event>>()};

   for(const auto &connection : mConnectionList)
   {
      time sendingTime = calc_sending_time();

      newEventList->push_back(
         event{get_node_id(), connection.mToNodeId, sendingTime});
   }

   return newEventList;
}

bool node::is_informed() const
{
   return mIsInformed;
}

time node::get_information_time() const
{
   return mInformationTime;
}

node_id node::get_node_id() const
{
   return mNodeId;
}

degree node::get_degree() const
{
   return mConnectionList.size();
}

const std::vector<connection> &node::get_connections() const
{
   return mConnectionList;
}

time node::calc_sending_time() const
{
   std::exponential_distribution<double> exponentialDistribution(mCharacteristicTime);
   return mInformationTime + exponentialDistribution(mRandomNumberGenerator);
}

} // namespace simulator
