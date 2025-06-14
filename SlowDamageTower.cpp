#include <cmath>
#include "SlowDamageTower.h"
#include "Critter.h"
#include "Bullet.h"

SlowDamageTower::SlowDamageTower(sf::Vector2f position)
    : Tower("SlowDamage", 120, 60, 5, 1, "./assets/slow_tower.png", position), slowEffect(0.8f) {
    range = 100;
    setScaleToTileSize(80);
}

void SlowDamageTower::attack(std::vector<Critter*>& critters, std::vector<Bullet>& bullets) {
    for (auto* critter : critters) {
        float dist = std::hypot(critter->getPosition().x - position.x, critter->getPosition().y - position.y);
        if (dist <= range) {
            critter->takeDamage(power);
            critter->slow(slowEffect);
            bullets.emplace_back(position, critter->getPosition());
            isFiring = true;
            firingTimer.restart();
            notify(TowerEventType::TowerAttacked);
            break;
        }
    }
}