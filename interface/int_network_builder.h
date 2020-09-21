#ifndef __INT_NETWORK_BUILDER_H__
#define __INT_NETWORK_BUILDER_H__

#include <memory>

namespace simulator
{

struct network_properties;
class int_network;

class int_network_builder
{
public:
   virtual std::unique_ptr<int_network>
      construct(const network_properties &networkProperties) = 0;
   virtual ~int_network_builder()
   {
   }
};

} // namespace simulator

#endif

