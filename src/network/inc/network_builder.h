#ifndef __NETWORK_BUILDER_H__
#define __NETWORK_BUILDER_H__

#include <memory>
#include <random>
#include <vector>

#include "int_degree_distribution.h"
#include "types.h"

namespace simulator
{

class network_builder final
{
public:
   explicit network_builder(std::mt19937 &randomNumberGenerator);
   std::unique_ptr<network>
      construct(const network_properties &networkProperties);

   node_list create_nodes(const network_properties &networkProperties) const;

   // factory method to create a degree distribution specified by type parameter
   std::unique_ptr<int_degree_distribution>
      create_degree_distribution(const network_properties &networkProperties) const;

   connectivity_result connect_nodes_randomly(
      const node_list &nodes,
      const std::unique_ptr<int_degree_distribution> &degreeDistribution,
      const bool allowLoops) const;

private:
   std::vector<degree> create_free_degree_pdf(
      const node_list &nodes,
      const std::unique_ptr<int_degree_distribution> &degreeDistribution) const;
   bool is_possible_to_create_connection(
      const std::vector<degree> &freeDegreePdf,
      const bool allowLoops) const;

   bool is_possible_to_create_connection_without_loops(
      const std::vector<degree> &freeDegreePdf) const;
   bool is_possible_to_create_connection_with_loops(
      const std::vector<degree> &freeDegreePdf) const;

   void create_random_connection(const node_list &nodes,
                                 std::vector<degree> &freeDegreePdf,
                                 const bool allowLoops) const;

   node_id
      get_node_id_for_random_degree(const std::vector<degree> &freeDegreePdf) const;

   network_builder (const network_builder&) = delete;
   network_builder& operator=(const network_builder&) = delete;
   network_builder& operator=(network_builder&&) = delete;

private:
   std::mt19937 &mRandomNumberGenerator;
};

} // namespace simulator

#endif
