//
// Created by Daniel Becher on 2/27/24.
//

#ifndef EVENTPOLLER_H
#define EVENTPOLLER_H

#include <deque>
#include "Event.h"

class EventPoller {
public:
    EventPoller() = default;
    bool Poll(RPG::Event* e);
    void Push(RPG::Event e);
private:
    std::deque<RPG::Event> EventQueue;
};



#endif //EVENTPOLLER_H
