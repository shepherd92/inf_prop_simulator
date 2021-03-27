#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <memory>

#include "types.h"

namespace simulator
{

class network final
{
public:
   network();

   void set_nodes(node_list &nodes);
   void set_transmissibility(const double transmissibility);
   const node_list &get_nodes() const;
   event_list inject_information_to_random_node(const time now);

   event_list inform_node(const node_id nodeId, const time now);
   bool is_informed(const node_id nodeId) const;

   size_t get_num_of_nodes() const;

   std::unique_ptr<result> get_result(const time timeOfInitialization) const;

private:
   node_list mNodes;
   double mTransmissibility;
};

} // namespace simulator

#endif
