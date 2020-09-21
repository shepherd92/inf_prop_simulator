#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

#include <queue>
#include <vector>

#include "types.h"

#include "int_event_queue.h"

namespace simulator
{

class event_queue final : public int_event_queue
{
public:
   explicit event_queue(const time now);

   virtual void update(event_list newEventList) override;
   virtual event next() override;

   virtual bool is_empty() const override;
   virtual time get_current_time() const override;
   virtual size_t get_num_of_events() const override;
   virtual void reset_clock() override;

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
