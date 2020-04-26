#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <memory>
#include <numeric>
#include <random>

#include "logger.h"
#include "network.h"
#include "network_builder.h"
#include "node.h"
#include "power_law_degree_distribution.h"
#include "types.h"
#include "uniform_degree_distribution.h"

namespace simulator {

network_builder::network_builder()
    : mRandomNumberGenerator{std::random_device{}()} {
  if (0.0 == std::random_device{}.entropy()) {
    mRandomNumberGenerator = std::mt19937(static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
  }
}

std::unique_ptr<int_network>
network_builder::construct(const network_properties &networkProperties) {
  LOG(DEBUG, "Network build started.");

  int_node_list nodes = create_nodes(networkProperties);
  const auto degreeDistribution = create_degree_distribution(networkProperties);
  connect_nodes_randomly(nodes, degreeDistribution);

  const double transmissibility = networkProperties.mTransmissibility;
  assert(transmissibility >= 0.0 && transmissibility <= 1.0);

  std::unique_ptr<int_network> networkProduct = std::make_unique<network>();
  networkProduct->set_nodes(nodes);
  networkProduct->set_transmissibility(transmissibility);
  LOG(DEBUG, "Network build finished.");

  degree sumOfDegrees = 0;
  for (const auto &node : networkProduct->get_nodes()) {
    sumOfDegrees += node->get_degree();
  }
  LOG(DEBUG, "Average of degrees is ",
      double(sumOfDegrees) / networkProperties.mNumOfNodes);

  return networkProduct;
}

int_node_list network_builder::create_nodes(
    const network_properties &networkProperties) const {
  int_node_list nodes =
      std::vector<std::unique_ptr<int_node>>(networkProperties.mNumOfNodes);

  for (node_id nodeId = 0U; nodeId < nodes.size(); ++nodeId) {
    nodes[nodeId] = std::make_unique<node>(nodeId);
  }

  return nodes;
}

std::unique_ptr<int_degree_distribution>
network_builder::create_degree_distribution(
    const network_properties &networkProperties) const {

  assert(networkProperties.mDegreeDistributionRange.mMinimum <=
         networkProperties.mDegreeDistributionRange.mMaximum);

  std::unique_ptr<int_degree_distribution> degreeDistribution;

  if (degree_distribution_type::uniform ==
      networkProperties.mDegreeDistributionType) {
    LOG(DEBUG, "Degree distribution is uniform ");
    degreeDistribution = std::make_unique<uniform_degree_distribution>(
        networkProperties.mDegreeDistributionRange);
  } else if (degree_distribution_type::power_law ==
             networkProperties.mDegreeDistributionType) {
    // in order to have a giant component in the network, the Molloy-Reed
    // criterion has to be fulfilled <k^2> - 2 <k> > 0
    // https://en.wikipedia.org/wiki/Configuration_model
    // in case of power law distribution, the criterion is as follows:
    // 1 < power < 4
    const double power = networkProperties.mDegreeDistributionPower;
    LOG(DEBUG, "Degree distribution is power law with power ", power);
    degreeDistribution = std::make_unique<power_law_degree_distribution>(
        networkProperties.mDegreeDistributionRange, power);
  } else {
    LOG(CRIT, "Unknown degree distribution type given.");
    assert(false);
  }

  degreeDistribution->generate_distribution();

  return degreeDistribution;
}

void network_builder::connect_nodes_randomly(
    const int_node_list &nodes,
    const std::unique_ptr<int_degree_distribution> &degreeDistribution) const {

  LOG(DEBUG, "Connecting nodes started.");

  // initialize the density function of the free degrees to be connected
  auto freeDegreePdf = create_free_degree_pdf(nodes, degreeDistribution);

  // create as many random connections in the graph as possible
  while (is_possible_to_create_connection(freeDegreePdf)) {
    create_random_connection(nodes, freeDegreePdf);
  }

  LOG(DEBUG, "Connecting nodes finished.");
}

std::vector<degree> network_builder::create_free_degree_pdf(
    const int_node_list &nodes,
    const std::unique_ptr<int_degree_distribution> &degreeDistribution) const {

  LOG(DEBUG, "Creating node degrees started.");

  // set node degrees randomly
  std::vector<degree> freeDegreePdf{};
  for (auto i = 0U; i < nodes.size(); ++i) {
    const degree randomDegree = degreeDistribution->get_random_degree();
    freeDegreePdf.push_back(randomDegree);
  }

  assert(freeDegreePdf.size() == nodes.size());
  LOG(DEBUG, "Creating node degrees finished.");

  return freeDegreePdf;
}

bool network_builder::is_possible_to_create_connection(
    const std::vector<degree> &freeDegreePdf) const {
  // check for at least two distinct, nonzero values in the degree probability
  // density function
  uint32_t numOfNonZeroElements = 0U;
  for (const auto pdfValue : freeDegreePdf) {

    if (0U != pdfValue) {
      ++numOfNonZeroElements;

      if (2U == numOfNonZeroElements) {
        return true;
      }
    }
  }
  return false;
}

void network_builder::create_random_connection(
    const int_node_list &nodes, std::vector<degree> &freeDegreePdf) const {
  // prerequisite of this function: there must be at least two nodes with free
  // degrees

  // create the two endpoints of the connection
  const node_id firstNodeId = get_node_id_for_random_degree(freeDegreePdf);
  const node_id secondNodeId = get_node_id_for_random_degree(freeDegreePdf);

  // prevent loops
  if (firstNodeId != secondNodeId) {
    LOG(DEBUG, "Creating connection between nodes ", firstNodeId, " and ",
        secondNodeId);
    // update probability density function
    --freeDegreePdf[firstNodeId];
    --freeDegreePdf[secondNodeId];

    // add connections to the nodes
    connection connectionFirstToSecond{connection::type::outgoing,
                                       secondNodeId};
    connection connectionSecondToFirst{connection::type::incoming, firstNodeId};
    nodes[firstNodeId]->add_connection(connectionFirstToSecond);
    nodes[secondNodeId]->add_connection(connectionSecondToFirst);
  }
}

node_id network_builder::get_node_id_for_random_degree(
    const std::vector<degree> &freeDegreePdf) const {
  // choose a random degree between 0 and the sum of the degrees
  std::uniform_int_distribution<> uniform_int_distribution(
      1U, std::accumulate(freeDegreePdf.begin(), freeDegreePdf.end(), 0U));
  const degree randomDegree = uniform_int_distribution(mRandomNumberGenerator);

  // choose the node ID which belongs to the randomly chosen degree
  // this is done through calculating the cumulative distribution function
  degree cumulativeSum = 0U;
  for (node_id nodeId = 0U; nodeId < freeDegreePdf.size(); ++nodeId) {
    cumulativeSum += freeDegreePdf[nodeId];
    if (cumulativeSum >= randomDegree) {
      return nodeId;
    }
  }

  // the node ID should have been found
  assert(false);
  return node_id{};
}

} // namespace simulator

