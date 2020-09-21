#include "uniform_degree_distribution.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <random>

#include "logger.h"
#include "types.h"

namespace simulator
{

uniform_degree_distribution::uniform_degree_distribution(
   const degree kMin, const degree kMax, std::mt19937 &randomNumberGenerator)
   : mRange()
   , mDistribution()
   , mRandomNumberGenerator {randomNumberGenerator}
{
   mRange.mMinimum = kMin;
   mRange.mMaximum = calculate_k_max(kMax);

   assert(mRange.mMinimum <= mRange.mMaximum);
}

void uniform_degree_distribution::generate_distribution()
{
   mDistribution = std::uniform_real_distribution<double>(mRange.mMinimum,
                                                          mRange.mMaximum + 1.0);
}

degree uniform_degree_distribution::get_random_degree() const
{
   const degree randomDegree {static_cast<degree>(std::floor(mDistribution(mRandomNumberGenerator)))};
   return randomDegree;
}

degree uniform_degree_distribution::calculate_k_max(const degree kMax)
{
   LOG(DEBUG, "Maximum degree is ", kMax);
   return kMax;
}

const degree_distribution_range &
uniform_degree_distribution::get_range() const
{
   return mRange;
}

const std::uniform_real_distribution<double> &
uniform_degree_distribution::get_distribution() const
{
   return mDistribution;
}

} // namespace simulator
