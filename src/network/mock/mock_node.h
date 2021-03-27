#ifndef __MOCK_NODE_H__
#define __MOCK_NODE_H__ 

#include <random>

#include "node.h"


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

void node::add_connection(const connection &)
{
}

bool node::is_connected_to(const node_id &) const
{
   return false;
}

event_list node::inform(const time)
{
   event_list newEventList {std::make_unique<std::vector<event>>()};
   return newEventList;
}

bool node::is_informed() const
{
   return false;
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
   return degree{0U};
}

const std::vector<connection> &node::get_connections() const
{
   return mConnectionList;
}

time node::calc_sending_time() const
{
   return 0.0;
}

} // namespace simulator

#endif