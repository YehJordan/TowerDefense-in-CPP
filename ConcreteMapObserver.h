#ifndef CONCRETE_MAP_OBSERVER_H
#define CONCRETE_MAP_OBSERVER_H

#include "MapObserver.h"
#include "Logger.h"

class ConcreteMapObserver : public MapObserver {
public:
    ConcreteMapObserver() = default;
    void update() override {
        logObserver("[Map Observer]: Map state has changed!");
    }
};

#endif