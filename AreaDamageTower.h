#ifndef AREA_DAMAGE_TOWER_H
#define AREA_DAMAGE_TOWER_H

#include "Tower.h"
#include <vector>
#include "Bullet.h" // Added

class AreaDamageTower : public Tower {
public:
    AreaDamageTower(sf::Vector2f position);
    void attack(std::vector<Critter*>& critters, std::vector<Bullet>& bullets) override;
};

#endif