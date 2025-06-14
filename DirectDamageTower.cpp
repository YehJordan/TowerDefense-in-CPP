#include <cmath>
#include "DirectDamageTower.h"
#include "Critter.h"
#include "Bullet.h"
#include <iostream>

DirectDamageTower::DirectDamageTower(sf::Vector2f position)
    : Tower("DirectDamage", 100, 50, 10, 1, "./assets/direct_tower.png", position) {
    range = 100;
    setScaleToTileSize(80);
}

void DirectDamageTower::attack(std::vector<Critter*>& critters, std::vector<Bullet>& bullets) {
    for (auto* critter : critters) {
        float dist = std::hypot(critter->getPosition().x - position.x, critter->getPosition().y - position.y);
        if (dist <= range) {
            critter->takeDamage(power);
            bullets.emplace_back(position, critter->getPosition()); // Spawn bullet
            isFiring = true;
            firingTimer.restart();
            notify(TowerEventType::TowerAttacked);
            break;
        }
    }
}