#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "types.h"

#include "node.h"

#include "int_degree_distribution.h"
#include "logger.h"

namespace simulator {

node::node(const node_id nodeId)
    : mNodeId(nodeId), mRandomNumberGenerator{std::random_device{}()} {
  if (0.0 == std::random_device{}.entropy()) {
    mRandomNumberGenerator = std::mt19937(static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
  }
}

void node::add_connection(const connection &connection) {
  mConnectionList.push_back(connection);
}

bool node::is_connected_to(const node_id &nodeId) const {
  for (const auto &connection : mConnectionList) {
    if (nodeId == connection.mToNodeId) {
      return true;
    }
  }
  return false;
}

event_list node::inform(const time now, const double transmissibility) {
  LOG(DEBUG, "Node ", mNodeId, " informed at time ", now);

  mInformationTime = now;
  
  event_list newEventList{std::make_unique<std::vector<event>>()};
  std::uniform_real_distribution<double> uniformDistribution(0.0, 1.0);

  for (const auto &connection : mConnectionList) {
    if(uniformDistribution(mRandomNumberGenerator) > transmissibility) {
      continue;
    }
    time sendingTime = calc_sending_time();
    newEventList->push_back(
        event{get_node_id(), connection.mToNodeId, sendingTime});
  }

  return newEventList;
}

bool node::is_informed() const { return mInformationTime.has_value(); }

std::optional<time> node::get_information_time() const {
  return mInformationTime;
}

node_id node::get_node_id() const { return mNodeId; }

degree node::get_degree() const { return mConnectionList.size(); }

const std::vector<connection> &node::get_connections() const {
  return mConnectionList;
}

time node::calc_sending_time() const {
  constexpr double mu{1.0};
  std::exponential_distribution<double> exponentialDistribution(mu);
  return mInformationTime.value() +
         exponentialDistribution(mRandomNumberGenerator);
}

} // namespace simulator
