#ifndef CRITTER_VIEW_H
#define CRITTER_VIEW_H

#include "CritterObserver.h"
#include "Critter.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>


class CritterView : public CritterObserver {
public:
    CritterView(sf::RenderWindow& window) : window(window) {}

    void addCritter(Critter* critter) {
        critters.push_back(critter);
        critter->addObserver(this); // Register as an observer for this critter
        critter->notifyObservers(CritterEventType::CritterAdded);
    }

    void removeCritter(Critter* critter) {
		critter->notifyObservers(CritterEventType::CritterRemoved);
        critter->removeObserver(this);
        critters.erase(std::remove(critters.begin(), critters.end(), critter), critters.end());
    }

    void updateView(CritterEventType event) override {
        if (event == CritterEventType::CritterAdded) {
            logObserver("[Critter Observer]: A new critter has been added!");
        } else if (event == CritterEventType::CritterDamaged) {
            logObserver("[Critter Observer]: A critter has taken damage!");
        } else if (event == CritterEventType::CritterKilled) {
            logObserver("[Critter Observer]: A critter has been killed!");
        } else if (event == CritterEventType::CritterReachedEnd) {
            logObserver("[Critter Observer]: A critter has reached the exit!");
        }

        for (Critter* critter : critters) {
            critter->draw(window);
        }
        window.display();
    }

private:
    sf::RenderWindow& window;
    std::vector<Critter*> critters;
};

#endif