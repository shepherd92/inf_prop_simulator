#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <memory>

#include "int_network.h"
#include "types.h"

namespace simulator
{

class network final : public int_network
{
public:
   network();

   virtual void set_nodes(int_node_list &nodes) override;
   virtual void set_transmissibility(const double transmissibility) override;
   virtual const int_node_list &get_nodes() const override;
   virtual event_list inject_information_to_random_node(const time now) override;

   virtual event_list inform_node(const node_id nodeId, const time now) override;
   virtual bool is_informed(const node_id nodeId) const override;

   virtual size_t get_num_of_nodes() const override;

   virtual std::unique_ptr<result> get_result(const time timeOfInitialization) const override;

private:
   int_node_list mNodes;
   double mTransmissibility;
};

} // namespace simulator

#endif
