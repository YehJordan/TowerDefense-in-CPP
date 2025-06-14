#ifndef TOWER_OBSERVER_H
#define TOWER_OBSERVER_H

class TowerObserver {
public:
    virtual ~TowerObserver() = default;
    virtual void update() = 0;

protected:
    TowerObserver() = default;
};

#endif