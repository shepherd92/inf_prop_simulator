#ifndef __INT_NETWORK_H__
#define __INT_NETWORK_H__

#include <memory>

#include "types.h"

namespace simulator
{

class int_network
{
public:
   virtual void set_nodes(int_node_list &nodes) = 0;
   virtual void set_transmissibility(const double transmissibility) = 0;
   virtual const int_node_list &get_nodes() const = 0;
   virtual event_list inject_information_to_random_node(const time now) = 0;

   virtual event_list inform_node(const node_id nodeId, const time now) = 0;
   virtual bool is_informed(const node_id nodeId) const = 0;

   virtual size_t get_num_of_nodes() const = 0;

   virtual std::unique_ptr<result> get_result(const time time_of_initialization) const = 0;

   virtual ~int_network()
   {
   }
};

} // namespace simulator

#endif
