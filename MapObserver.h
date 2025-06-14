#ifndef MAP_OBSERVER_H
#define MAP_OBSERVER_H

#include "MapEvents.h" // Include this for EventType if needed elsewhere

class Map; // Forward declaration instead of including Map.h

class MapObserver {
public:
    virtual ~MapObserver(); // Define destructor in .cpp to use Map
    virtual void update() = 0;
    void setSubject(Map* map_subject) { subject = map_subject; }

protected:
    MapObserver() : subject(nullptr) {}
    Map* subject;
};

#endif