#ifndef CRITTER_GROUP_GENERATOR_H
#define CRITTER_GROUP_GENERATOR_H

#include "Critter.h"
#include <vector>

class CritterGroupGenerator {
public:
    CritterGroupGenerator();
    std::vector<Critter*> generateWave(int waveNumber, const sf::Vector2f& startPos, float tileSize);

private:
    Critter* createCritter(int waveNumber, const sf::Vector2f& startPos, float tileSize); // Factory method
};

#endif