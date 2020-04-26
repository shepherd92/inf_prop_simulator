#ifndef __UNIFORM_DEGREE_DISTRIBUTION__
#define __UNIFORM_DEGREE_DISTRIBUTION__

#include <random>

#include "types.h"

#include "int_degree_distribution.h"

namespace simulator {

class uniform_degree_distribution final : public int_degree_distribution {
public:
  explicit uniform_degree_distribution(const degree_distribution_range range);
  virtual void generate_distribution() override;
  virtual degree get_random_degree() const override;

private:
  const degree_distribution_range mRange;
  mutable std::uniform_real_distribution<double> mDistribution;
  mutable std::mt19937 mRandomNumberGenerator;
};

} // namespace simulator

#endif
