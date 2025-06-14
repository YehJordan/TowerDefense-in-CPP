#include "MapObserver.h"
#include "Map.h"

MapObserver::~MapObserver() {
    if (subject) subject->detach(this);
}