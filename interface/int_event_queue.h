#ifndef __INT_EVENT_QUEUE_H__
#define __INT_EVENT_QUEUE_H__

#include "types.h"

namespace simulator
{

class int_event_queue
{
public:
   virtual void update(event_list newEventList) = 0;
   virtual event next() = 0;

   virtual bool is_empty() const = 0;
   virtual time get_current_time() const = 0;
   virtual size_t get_num_of_events() const = 0;
   virtual void reset_clock() = 0;

   virtual ~int_event_queue() = default;
};

} // namespace simulator

#endif
