#ifndef __MOCK_POISSON_DEGREE_DISTRIBUTION_H__
#define __MOCK_POISSON_DEGREE_DISTRIBUTION_H__ 


#include "poisson_degree_distribution.h"


namespace simulator
{
    
poisson_degree_distribution::poisson_degree_distribution(
   const degree, const double parameter, const uint32_t, std::mt19937 &randomNumberGenerator)
   : mRange()
   , mParameter(parameter)
   , mDistribution()
   , mRandomNumberGenerator {randomNumberGenerator}
{
}

void poisson_degree_distribution::generate_distribution()
{
}

degree poisson_degree_distribution::get_random_degree() const
{
   return degree{0U};
}

} // namespace simulator

#endif