#include <cassert>
#include <cmath>
#include <cstdint>
#include <random>
#include <vector>

#include "logger.h"
#include "types.h"

#include "power_law_degree_distribution.h"

namespace simulator
{

power_law_degree_distribution::power_law_degree_distribution(
   const degree kMin,
   const double parameter,
   const uint32_t numOfNodes,
   std::mt19937 &randomNumberGenerator)
   : mRange()
   , mParameter(parameter)
   , mDistribution()
   , mRandomNumberGenerator {randomNumberGenerator}
{
   mRange.mMinimum = kMin;
   mRange.mMaximum = calculate_k_max(numOfNodes);

   assert(mRange.mMinimum <= mRange.mMaximum);
   assert(mParameter > 0.0F);
}

void power_law_degree_distribution::generate_distribution()
{
   std::vector<uint32_t> degrees(mRange.mMaximum - mRange.mMinimum + 2U);
   degrees[0] = mRange.mMinimum - 1U;
   for(degree degree = mRange.mMinimum; degree < mRange.mMaximum + 1U;
       ++degree)
   {
      degrees[degree - mRange.mMinimum + 1U] = degree;
   }

   std::vector<double> weights(mRange.mMaximum - mRange.mMinimum + 1U);
   for(auto degree = mRange.mMinimum; degree < mRange.mMaximum + 1U; ++degree)
   {
      weights[degree - mRange.mMinimum] = std::pow(degree, -mParameter);
   }

   mDistribution = std::piecewise_constant_distribution<double>(
      degrees.begin(), degrees.end(), weights.begin());
}

degree power_law_degree_distribution::get_random_degree() const
{
   const degree randomDegree {static_cast <degree> (std::ceil(mDistribution(mRandomNumberGenerator)))};
   return randomDegree;
}

degree power_law_degree_distribution::calculate_k_max(const uint32_t numOfNodes)
{
   const degree kMax {static_cast<degree>(mRange.mMinimum * pow(numOfNodes, 1.0 /
      (mParameter - 1.0)))};

   LOG(DEBUG, "Maximum degree is ", kMax);
   return kMax;
}

const degree_distribution_range &
power_law_degree_distribution::get_range() const
{
   return mRange;
}

double power_law_degree_distribution::get_parameter() const
{
   return mParameter;
}

const std::piecewise_constant_distribution<double> &
power_law_degree_distribution::get_distribution() const
{
   return mDistribution;
}

} // namespace simulator

