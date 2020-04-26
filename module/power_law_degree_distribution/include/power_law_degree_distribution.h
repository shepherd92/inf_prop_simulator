#ifndef __POWER_LAW_DEGREE_DISTRIBUTION__
#define __POWER_LAW_DEGREE_DISTRIBUTION__

#include <random>

#include "types.h"

#include "int_degree_distribution.h"

namespace simulator {

class power_law_degree_distribution final : public int_degree_distribution {
public:
  explicit power_law_degree_distribution(const degree_distribution_range range,
                                         const double power);

  virtual degree get_random_degree() const override;
  virtual void generate_distribution() override;

  const degree_distribution_range &get_range() const;
  double get_power() const;
  const std::piecewise_constant_distribution<double> &get_distribution() const;

private:
  const degree_distribution_range mRange;
  const double mPower;
  mutable std::piecewise_constant_distribution<double> mDistribution;
  mutable std::mt19937 mRandomNumberGenerator;
};

} // namespace simulator

#endif
