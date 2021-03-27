#include <cassert>
#include <cmath>
#include <cstdint>
#include <random>
#include <vector>

#include "logger.h"
#include "types.h"

#include "poisson_degree_distribution.h"

namespace simulator
{

poisson_degree_distribution::poisson_degree_distribution(
   const degree kMin, const double parameter, const uint32_t numOfNodes, std::mt19937 &randomNumberGenerator)
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

void poisson_degree_distribution::generate_distribution()
{

   std::vector<double> degrees(mRange.mMaximum - mRange.mMinimum + 2U);
   degrees[0] = mRange.mMinimum - 1U;
   for(auto degree {mRange.mMinimum}; degree < mRange.mMaximum + 1U;
       ++degree)
   {
      degrees[degree - mRange.mMinimum + 1U] = degree;
   }

   std::vector<double> weights(mRange.mMaximum - mRange.mMinimum + 1U);
   for(auto degree {mRange.mMinimum}; degree < mRange.mMaximum + 1U; ++degree)
   {
      weights[degree - mRange.mMinimum] = std::pow(mParameter, degree) / tgamma(degree + 1.0);
   }

   mDistribution = std::piecewise_constant_distribution<double>(
      degrees.begin(), degrees.end(), weights.begin());
}

degree poisson_degree_distribution::get_random_degree() const
{
   const auto randomDegree {static_cast<degree>(std::ceil(mDistribution(mRandomNumberGenerator)))};
   return randomDegree;
}

degree poisson_degree_distribution::calculate_k_max(const uint32_t numOfNodes)
{
   // calculate the CDF of the Poisson degree distribution until it becomes greater than 1 - 1/N
   double cdf_without_exponential {0.0};
   int32_t kMax {-1};

   while(cdf_without_exponential < exp(mParameter) * (1.0 - 1.0 / numOfNodes))
   {
      ++kMax;
      cdf_without_exponential += pow(mParameter, kMax) / tgamma(kMax + 1);
   }

   LOG(DEBUG, "Maximum degree is ", kMax);
   return static_cast <degree> (kMax);
}

const degree_distribution_range &
poisson_degree_distribution::get_range() const
{
   return mRange;
}

double poisson_degree_distribution::get_parameter() const
{
   return mParameter;
}

const std::piecewise_constant_distribution<double> &
poisson_degree_distribution::get_distribution() const
{
   return mDistribution;
}

} // namespace simulator

