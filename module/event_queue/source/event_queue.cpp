#include "event_queue.h"

#include "logger.h"
#include "types.h"

namespace simulator {

event_queue::event_queue(const time now) : mClock(now) {}

void event_queue::update(event_list newEventList) {
  for (const auto &event : *newEventList) {
    mEventQueue.push(event);
  }
}

event event_queue::next() {
  event nextEvent = mEventQueue.top();
  mEventQueue.pop();
  mClock = nextEvent.mTime;
  return nextEvent;
}

bool event_queue::is_empty() const { return mEventQueue.empty(); }

time event_queue::get_current_time() const { return mClock; }

} // namespace simulator
