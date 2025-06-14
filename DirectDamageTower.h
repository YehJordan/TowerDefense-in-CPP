#ifndef DIRECT_DAMAGE_TOWER_H
#define DIRECT_DAMAGE_TOWER_H

#include "Tower.h"
#include <vector>
#include "Bullet.h"

class DirectDamageTower : public Tower {
public:
    DirectDamageTower(sf::Vector2f position);
    void attack(std::vector<Critter*>& critters, std::vector<Bullet>& bullets) override;
};

#endif