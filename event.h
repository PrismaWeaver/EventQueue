#ifndef EVENT_H
#define EVENT_H

#include "process.h"

struct Event {
    int type;
    long double time;
    Event* nextEvent;
    Process * p;

    Event(int ty, long double ti, Process *process){
        type = ty;
        time = ti;
        p = process;
        nextEvent = NULL;
    }
};

#endif