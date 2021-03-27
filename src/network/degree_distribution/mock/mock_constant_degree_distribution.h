#ifndef __MOCK_CONSTANT_DEGREE_DISTRIBUTION_H__
#define __MOCK_CONSTANT_DEGREE_DISTRIBUTION_H__ 


#include "constant_degree_distribution.h"


namespace simulator
{
    
constant_degree_distribution::constant_degree_distribution(const degree)
   : mRange()
{
}

void constant_degree_distribution::generate_distribution()
{
}

degree constant_degree_distribution::get_random_degree() const
{
   return degree{0U};
}

} // namespace simulator

#endif