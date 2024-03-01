//
// Created by Daniel Becher on 2/27/24.
//

#include "EventPoller.h"

/**
 * Polls for the event queue. Populates e with the next event in line if any events
 * \param e The event struct to be populated if the queue is not empty. This event is then popped.
 * \return True if the queue was not empty, false if it is.
 */
bool EventPoller::Poll(RPG::Event *e) {
    if (EventQueue.empty()) return false;
    *e = EventQueue.front();
    EventQueue.pop_front();
    return true;
}
