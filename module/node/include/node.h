#ifndef __NODE_H__
#define __NODE_H__

#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "types.h"

#include "int_node.h"

namespace simulator {

class node final : public int_node {
public:
  explicit node(const node_id nodeId);

  virtual void add_connection(const connection &connection) override;
  virtual bool is_connected_to(const node_id &nodeId) const override;

  virtual event_list inform(const time now, const double prevention) override;
  virtual bool is_informed() const override;

  virtual std::optional<time> get_information_time() const override;

  virtual node_id get_node_id() const override;
  virtual degree get_degree() const override;

  virtual const std::vector<connection> &get_connections() const override;

private:
  time calc_sending_time() const;

  const node_id mNodeId;
  std::optional<time> mInformationTime{};
  std::vector<connection> mConnectionList{};

  mutable std::mt19937 mRandomNumberGenerator;
};

} // namespace simulator

#endif
