#include "uniform_degree_distribution.h"

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <random>

#include "types.h"

namespace simulator {

uniform_degree_distribution::uniform_degree_distribution(
    const degree_distribution_range range)
    : mRange(range),
      mDistribution(), mRandomNumberGenerator{std::random_device{}()} {
  if (0.0 == std::random_device{}.entropy()) {
    mRandomNumberGenerator = std::mt19937(static_cast<uint64_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
  }

  assert(mRange.mMinimum <= mRange.mMaximum);
}

void uniform_degree_distribution::generate_distribution() {
  mDistribution = std::uniform_real_distribution<double>(mRange.mMinimum,
                                                         mRange.mMaximum + 1.0);
}

degree uniform_degree_distribution::get_random_degree() const {
  const degree randomDegree = std::floor(mDistribution(mRandomNumberGenerator));
  return randomDegree;
}

} // namespace simulator
