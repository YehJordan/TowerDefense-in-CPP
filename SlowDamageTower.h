#ifndef SLOW_DAMAGE_TOWER_H
#define SLOW_DAMAGE_TOWER_H

#include "Tower.h"
#include <vector>
#include "Bullet.h" // Added

class SlowDamageTower : public Tower {
private:
    float slowEffect;

public:
    SlowDamageTower(sf::Vector2f position);
    void attack(std::vector<Critter*>& critters, std::vector<Bullet>& bullets) override;
};

#endif