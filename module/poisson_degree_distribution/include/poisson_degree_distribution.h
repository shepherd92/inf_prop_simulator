#ifndef __POISSON_DEGREE_DISTRIBUTION_H__
#define __POISSON_DEGREE_DISTRIBUTION_H__

#include <random>

#include "types.h"

#include "int_degree_distribution.h"

namespace simulator
{

class poisson_degree_distribution final : public int_degree_distribution
{
public:
   explicit poisson_degree_distribution(const degree kMin,
                                        const double parameter,
                                        const uint32_t numOfNodes,
                                        std::mt19937 &randomNumberGenerator);

   virtual void generate_distribution() override;
   virtual degree get_random_degree() const override;

   const degree_distribution_range &get_range() const;
   double get_parameter() const;
   const std::piecewise_constant_distribution<double> &get_distribution() const;

private:
   degree calculate_k_max(const uint32_t numOfNodes);

   degree_distribution_range mRange;
   const double mParameter;
   mutable std::piecewise_constant_distribution<double> mDistribution;
   std::mt19937 &mRandomNumberGenerator;
};

} // namespace simulator

#endif
