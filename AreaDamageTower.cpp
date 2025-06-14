#include "AreaDamageTower.h"
#include "Bullet.h"
#include <iostream>
#include <cmath>

AreaDamageTower::AreaDamageTower(sf::Vector2f position)
    : Tower("AreaDamage", 150, 75, 8, 2, "./assets/area_tower.png", position) {
    range = 150;
    setScaleToTileSize(80);
}

void AreaDamageTower::attack(std::vector<Critter*>& critters, std::vector<Bullet>& bullets) {
    for (auto* critter : critters) {
        float dist = std::hypot(critter->getPosition().x - position.x, critter->getPosition().y - position.y);
        if (dist <= range && !critter->isDead()) {
            critter->takeDamage(power);
            bullets.emplace_back(position, critter->getPosition());
            isFiring = true;
            firingTimer.restart();
            notify(TowerEventType::TowerAttacked);
        }
    }
}