#ifndef __INT_NODE_H__
#define __INT_NODE_H__

#include "types.h"

namespace simulator
{

class int_node
{
public:
   virtual void add_connection(const connection &connection) = 0;
   virtual bool is_connected_to(const node_id &nodeId) const = 0;

   virtual event_list inform(const time now) = 0;
   virtual bool is_informed() const = 0;

   virtual time get_information_time() const = 0;

   virtual node_id get_node_id() const = 0;
   virtual degree get_degree() const = 0;

   virtual const std::vector<connection> &get_connections() const = 0;

   virtual ~int_node()
   {
   }
};

} // namespace simulator

#endif

