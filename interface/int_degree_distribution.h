#ifndef __INT_DEGREE_DISTRIBUTION_H__
#define __INT_DEGREE_DISTRIBUTION_H__

#include "types.h"

namespace simulator
{

class int_degree_distribution
{
public:
   virtual void generate_distribution() = 0;
   virtual degree get_random_degree() const = 0;
   virtual ~int_degree_distribution()
   {
   }
};

} // namespace simulator

#endif
