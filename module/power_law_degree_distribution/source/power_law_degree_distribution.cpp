#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <random>
#include <vector>

#include "types.h"

#include "power_law_degree_distribution.h"

namespace simulator {

power_law_degree_distribution::power_law_degree_distribution(
    const degree_distribution_range range, const double power)
    : mRange(range), mPower(power),
      mDistribution(), mRandomNumberGenerator{std::random_device{}()} {
  if (0.0 == std::random_device{}.entropy()) {
    mRandomNumberGenerator = std::mt19937(static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
  }
  assert(mRange.mMinimum <= mRange.mMaximum);
  assert(mPower > 0.0F);
}

void power_law_degree_distribution::generate_distribution() {

  std::vector<double> degrees(mRange.mMaximum - mRange.mMinimum + 2U);
  degrees[0] = mRange.mMinimum - 1U;
  for (double degree = mRange.mMinimum; degree < mRange.mMaximum + 1U;
       ++degree) {
    degrees[degree - mRange.mMinimum + 1U] = degree;
  }

  std::vector<double> weights(mRange.mMaximum - mRange.mMinimum + 1U);
  for (auto degree = mRange.mMinimum; degree < mRange.mMaximum + 1U; ++degree) {
    weights[degree - mRange.mMinimum] = std::pow(degree, -mPower);
  }

  mDistribution = std::piecewise_constant_distribution<double>(
      degrees.begin(), degrees.end(), weights.begin());
}

degree power_law_degree_distribution::get_random_degree() const {
  const degree randomDegree = std::ceil(mDistribution(mRandomNumberGenerator));
  return randomDegree;
}

const degree_distribution_range &
power_law_degree_distribution::get_range() const {
  return mRange;
}

double power_law_degree_distribution::get_power() const { return mPower; }

const std::piecewise_constant_distribution<double> &
power_law_degree_distribution::get_distribution() const {
  return mDistribution;
}

} // namespace simulator
