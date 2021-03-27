#ifndef __MOCK_POWER_LAW_DEGREE_DISTRIBUTION_H__
#define __MOCK_POWER_LAW_DEGREE_DISTRIBUTION_H__ 

#include "power_law_degree_distribution.h"

namespace simulator
{

power_law_degree_distribution::power_law_degree_distribution(
   const degree,
   const double parameter,
   const uint32_t,
   std::mt19937 &randomNumberGenerator)
   : mRange()
   , mParameter(parameter)
   , mDistribution()
   , mRandomNumberGenerator {randomNumberGenerator}
{
}

void power_law_degree_distribution::generate_distribution()
{
}

degree power_law_degree_distribution::get_random_degree() const
{
   return degree{0U};
}

} // namespace simulator

#endif