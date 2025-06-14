#ifndef SIMPLE_TOWER_OBSERVER_H
#define SIMPLE_TOWER_OBSERVER_H

#include "TowerObserver.h"
#include "Logger.h"

class SimpleTowerObserver : public TowerObserver {
public:
    SimpleTowerObserver() = default;
    void update() override {
        logObserver("[TowerObserver]: Tower state has changed!");
    }
};

#endif