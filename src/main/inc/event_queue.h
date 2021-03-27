#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

#include <queue>
#include <vector>

#include "types.h"

namespace simulator
{

class event_queue final
{
public:
   explicit event_queue(const time now);

   void update(event_list newEventList);
   event next();

   bool is_empty() const;
   time get_current_time() const;
   size_t get_num_of_events() const;
   void reset_clock();

private:
   struct compare_events
   {
      bool operator()(const event &event1, const event &event2)
      {
         return event1.mTime > event2.mTime;
      }
   };

   time mClock;
   std::priority_queue<event, std::vector<event>, compare_events> mEventQueue {};
};

} // namespace simulator

#endif
