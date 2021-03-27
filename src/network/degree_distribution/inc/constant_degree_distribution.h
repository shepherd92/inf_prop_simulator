#ifndef __CONSTANT_DEGREE_DISTRIBUTION_H__
#define __CONSTANT_DEGREE_DISTRIBUTION_H__

#include "types.h"
#include "logger.h"

#include "int_degree_distribution.h"

namespace simulator
{

class constant_degree_distribution final : public int_degree_distribution
{
public:
   explicit constant_degree_distribution(const degree kMin);
   virtual void generate_distribution() override;
   virtual degree get_random_degree() const override;

   const degree_distribution_range &get_range() const;

   constant_degree_distribution (const constant_degree_distribution&) = delete;
   constant_degree_distribution& operator=(const constant_degree_distribution&) = delete;
   constant_degree_distribution& operator=(constant_degree_distribution&&) = delete;

private:
   degree calculate_k_max();

   degree_distribution_range mRange;
};

} // namespace simulator

#endif
