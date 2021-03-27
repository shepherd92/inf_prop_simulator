#include "constant_degree_distribution.h"

#include <cassert>
#include <cstdint>

#include "logger.h"
#include "types.h"

namespace simulator
{

constant_degree_distribution::constant_degree_distribution(const degree kMin)
   : mRange()
{
   mRange.mMinimum = kMin;
   mRange.mMaximum = calculate_k_max();

   assert(mRange.mMinimum <= mRange.mMaximum);
}

void constant_degree_distribution::generate_distribution()
{
   // do nothing as the distribution is constant
   return;
}

degree constant_degree_distribution::get_random_degree() const
{
   const degree randomDegree {mRange.mMinimum};
   return randomDegree;
}

degree constant_degree_distribution::calculate_k_max()
{
   const degree kMax {mRange.mMinimum};
   LOG(DEBUG, "Maximum degree is ", kMax);
   return kMax;
}

const degree_distribution_range &
constant_degree_distribution::get_range() const
{
   return mRange;
}

} // namespace simulator
