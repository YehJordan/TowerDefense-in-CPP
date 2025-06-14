#include "CritterGroupGenerator.h"
#include "Critter.h"
#include "CritterTypes.h"
#include <iostream> // Kept for potential debugging


CritterGroupGenerator::CritterGroupGenerator() {}

std::vector<Critter*> CritterGroupGenerator::generateWave(int waveNumber, const sf::Vector2f& startPos, float tileSize) {
    std::vector<Critter*> critters;
    int numCritters = 1 + waveNumber / 5; // More critters every 5 waves
    for (int i = 0; i < numCritters; ++i) {
        Critter* critter = createCritter(waveNumber, startPos, tileSize);
        critter->notifyObservers(CritterEventType::CritterAdded);
        critters.push_back(critter);
    }
    return critters;
}

Critter* CritterGroupGenerator::createCritter(int waveNumber, const sf::Vector2f& startPos, float tileSize) {
    // Base stats increase with wave number
    int baseHP = 50 + waveNumber * 10;
    int baseSpeed = 1 + waveNumber;
    int reward = 10 + waveNumber * 2;
    int strength = 5 + waveNumber * 3;

    // Decide critter type based on wave number
    if (waveNumber < 3) {
        return new BasicCritter(baseHP, baseSpeed, reward, strength, "./assets/critter.png", startPos, tileSize);
    }
    else if (waveNumber < 6) {
        return new FastCritter(baseHP, baseSpeed, reward, strength, "./assets/fast.png", startPos, tileSize);
    }
    else {
        return new ToughCritter(baseHP, baseSpeed, reward, strength, "./assets/tough.png", startPos, tileSize);
    }
}