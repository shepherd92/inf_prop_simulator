#ifndef __UNIFORM_DEGREE_DISTRIBUTION_H__
#define __UNIFORM_DEGREE_DISTRIBUTION_H__

#include <random>

#include "types.h"

#include "int_degree_distribution.h"

namespace simulator
{

class uniform_degree_distribution final : public int_degree_distribution
{
public:
   explicit uniform_degree_distribution(const degree kMin, const degree kMax, std::mt19937 &randomNumberGenerator);
   virtual void generate_distribution() override;
   virtual degree get_random_degree() const override;

   const degree_distribution_range &get_range() const;
   const std::uniform_int_distribution<degree> &get_distribution() const;

   uniform_degree_distribution (const uniform_degree_distribution&) = delete;
   uniform_degree_distribution& operator=(const uniform_degree_distribution&) = delete;
   uniform_degree_distribution& operator=(uniform_degree_distribution&&) = delete;

private:
   degree calculate_k_max(const degree kMax);

   degree_distribution_range mRange;
   mutable std::uniform_int_distribution<degree> mDistribution;
   std::mt19937 &mRandomNumberGenerator;
};

} // namespace simulator

#endif
