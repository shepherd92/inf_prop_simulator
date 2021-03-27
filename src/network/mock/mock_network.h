#ifndef __MOCK_NETWORK_H__
#define __MOCK_NETWORK_H__ 


#include "network.h"

namespace simulator
{

network::network() : mNodes {}, mTransmissibility {0.0}
{
}

void network::set_nodes(node_list &)
{
}

void network::set_transmissibility(const double)
{
}

const node_list &network::get_nodes() const
{
   return mNodes;
}

event_list network::inject_information_to_random_node(const time)
{
   event_list initialEvents {};
   return initialEvents;
}

event_list network::inform_node(const node_id, const time)
{
   event_list events {};
   return events;
}

bool network::is_informed(const node_id) const
{
   return false;
}

size_t network::get_num_of_nodes() const
{
   return mNodes.size();
}

std::unique_ptr<result> network::get_result(const time) const
{
   auto simualtionResult = std::make_unique<result>();
   return simualtionResult;
}

} // namespace simulator

#endif