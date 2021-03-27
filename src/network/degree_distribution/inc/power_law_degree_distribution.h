#ifndef __POWER_LAW_DEGREE_DISTRIBUTION_H__
#define __POWER_LAW_DEGREE_DISTRIBUTION_H__

#include <random>

#include "types.h"

#include "int_degree_distribution.h"

namespace simulator
{

class power_law_degree_distribution final : public int_degree_distribution
{
public:
   explicit power_law_degree_distribution(const degree kMin,
                                          const double parameter,
                                          const uint32_t numOfNodes,
                                          std::mt19937 &randomNumberGenerator);

   virtual void generate_distribution() override;
   virtual degree get_random_degree() const override;

   const degree_distribution_range &get_range() const;
   double get_parameter() const;
   const std::piecewise_constant_distribution<double> &get_distribution() const;

   power_law_degree_distribution (const power_law_degree_distribution&) = delete;
   power_law_degree_distribution& operator=(const power_law_degree_distribution&) = delete;
   power_law_degree_distribution& operator=(power_law_degree_distribution&&) = delete;

private:
   degree calculate_k_max(const uint32_t numOfNodes);

   degree_distribution_range mRange;
   const double mParameter;
   mutable std::piecewise_constant_distribution<double> mDistribution;
   std::mt19937 &mRandomNumberGenerator;
};

} // namespace simulator

#endif
