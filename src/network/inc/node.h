#ifndef __NODE_H__
#define __NODE_H__

#include <memory>
#include <random>
#include <vector>

#include "types.h"

namespace simulator
{

class node
{
public:
   explicit node(const node_id nodeId, const double characteristicTime, std::mt19937 &randomNumberGenerator);

   void add_connection(const connection &connection);
   bool is_connected_to(const node_id &nodeId) const;

   event_list inform(const time now);
   bool is_informed() const;

   time get_information_time() const;

   node_id get_node_id() const;
   degree get_degree() const;

   const std::vector<connection> &get_connections() const;

   time calc_sending_time() const;

   node (const node&) = default;
   node& operator=(const node&) = delete;
   node& operator=(node&&) = delete;

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
