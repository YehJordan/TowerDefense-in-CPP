#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Critter.h"
#include "TowerObserver.h"
#include "TowerEvents.h"
#include "Bullet.h"

class Tower {
protected:
    std::string name;
    int level;
    int cost;
    int refund_value;
    int range;
    int power;
    float rate_of_fire; // Changed to float: shots per second
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    std::vector<TowerObserver*> observers;
    bool isFiring;
    sf::Clock firingTimer;
    float firingAnimationDuration;

public:
    Tower(std::string name, int cost, int refund, int power, float rateOfFire, const std::string& textureFile, sf::Vector2f position);
    virtual ~Tower() = default;
    virtual void attack(std::vector<Critter*>& critters, std::vector<Bullet>& bullets) = 0;
    void upgrade(int& playerCoins);
    void sell(int& playerCoins);
    void draw(sf::RenderWindow& window) const;
    void setScaleToTileSize(float tileSize);

    int getLevel() const { return level; }
    int getCost() const { return cost; }
    int getRefundValue() const { return refund_value; }
    int getRange() const { return range; }
    int getPower() const { return power; }
    float getRateOfFire() const { return rate_of_fire; } // Changed to float
    sf::Vector2f getPosition() const { return position; }
    std::string getName() const { return name; }
    void updateAnimation();
    void attach(TowerObserver* observer);
    void detach(TowerObserver* observer);
    void notify(TowerEventType event);

protected:
    sf::Vector2f getTileSize() const;
};

#endif