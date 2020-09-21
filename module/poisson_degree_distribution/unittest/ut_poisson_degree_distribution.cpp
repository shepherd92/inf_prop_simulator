#include <cassert>
#include <iostream>
#include <random>
#include <string>

#include "poisson_degree_distribution.h"
#include "types.h"

// undefine LOG macro to not do anything
#undef LOG
#define LOG(level, ...) ;

namespace simulator
{

bool essentially_equal(const double a, const double b)
{
   return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) *
                          std::numeric_limits<double>::epsilon());
}

void test_constructor()
{
   std::mt19937 randomNumberGenerator {std::random_device{}()};
   poisson_degree_distribution testObj {degree{1U}, 2.0, 1000U, randomNumberGenerator};
   assert(1U == testObj.get_range().mMinimum);
   assert(essentially_equal(testObj.get_parameter(), 2.0));
}

} // namespace simulator

int main()
{
   using namespace simulator;

   test_constructor();

   return 0;
}
