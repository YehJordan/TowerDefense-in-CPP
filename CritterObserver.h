#ifndef CRITTER_OBSERVER_H
#define CRITTER_OBSERVER_H
#include "Critter.h"
class CritterObserver {
public:
    virtual void updateView(CritterEventType event) = 0; // Changed to updateView
    virtual ~CritterObserver() = default;
};
#endif