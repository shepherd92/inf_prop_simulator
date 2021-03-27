#ifndef __MOCK_UNIFORM_DEGREE_DISTRIBUTION_H__
#define __MOCK_UNIFORM_DEGREE_DISTRIBUTION_H__ 


#include "uniform_degree_distribution.h"


namespace simulator
{

uniform_degree_distribution::uniform_degree_distribution(
   const degree, const degree, std::mt19937 &randomNumberGenerator)
   : mRange()
   , mDistribution()
   , mRandomNumberGenerator {randomNumberGenerator}
{
}

void uniform_degree_distribution::generate_distribution()
{
}

degree uniform_degree_distribution::get_random_degree() const
{
   return degree{0U};
}

} // namespace simulator

#endif