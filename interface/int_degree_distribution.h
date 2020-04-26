#ifndef __I_DEGREE_DISTRIBUTION_H__
#define __I_DEGREE_DISTRIBUTION_H__

#include "types.h"

namespace simulator {

class int_degree_distribution {
public:
  virtual degree get_random_degree() const = 0;
  virtual ~int_degree_distribution() {}
  virtual void generate_distribution() = 0;
};

} // namespace simulator

#endif
