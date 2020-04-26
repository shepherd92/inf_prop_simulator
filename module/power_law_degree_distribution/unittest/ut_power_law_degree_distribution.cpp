#include <cassert>
#include <iostream>
#include <string>

#include "power_law_degree_distribution.h"
#include "types.h"

namespace simulator {

bool essentially_equal(const double a, const double b) {
  return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) *
                         std::numeric_limits<double>::epsilon());
}

void test_constructor() {
  power_law_degree_distribution testObj{degree_distribution_range{1U, 3U}, 2.0};
  assert(1U == testObj.get_range().mMinimum);
  assert(3U == testObj.get_range().mMaximum);
  assert(essentially_equal(testObj.get_power(), 2.0));
  assert(3U == testObj.get_distribution().intervals().size());
}

void test_get_random_degree() {
  // generate numbers from constant distribution and check if they are the same
  power_law_degree_distribution testObj{degree_distribution_range{3U, 3U}, 2.0};
  testObj.generate_distribution();

  for (auto i = 0U; i < 100U; ++i) {
    assert(testObj.get_random_degree() == 3U);
  }
}

} // namespace simulator

int main() {
  using namespace simulator;

  // test_constructor();
  test_get_random_degree();

  return 0;
}
