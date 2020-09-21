#ifndef __NODE_H__
#define __NODE_H__

#include <memory>
#include <vector>

#include "types.h"

#include "int_node.h"

namespace simulator
{

class node final : public int_node
{
public:
   explicit node(const node_id nodeId, const double characteristicTime, std::mt19937 &randomNumberGenerator);

   virtual void add_connection(const connection &connection) override;
   virtual bool is_connected_to(const node_id &nodeId) const override;

   virtual event_list inform(const time now) override;
   virtual bool is_informed() const override;

   virtual time get_information_time() const override;

   virtual node_id get_node_id() const override;
   virtual degree get_degree() const override;

   virtual const std::vector<connection> &get_connections() const override;

   time calc_sending_time() const;

private:
   const node_id mNodeId;
   const double mCharacteristicTime;

   bool mIsInformed {false};
   time mInformationTime {std::numeric_limits<double>::quiet_NaN()};
   std::vector<connection> mConnectionList {};

   std::mt19937 &mRandomNumberGenerator;
};

} // namespace simulator

#endif
