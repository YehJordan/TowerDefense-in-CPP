#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Tower.h"
#include "MapEvents.h"
#include "MapObserver.h"
#include <fstream>
#include <iostream>

using namespace std;

class DirectDamageTower;
class AreaDamageTower;
class SlowDamageTower;

enum class TowerType { 
    DirectDamage, 
    AreaDamage, 
    SlowDamage 
};

class Map {
private:
    int width, height;
    std::vector<std::vector<sf::RectangleShape>> grid;
    std::vector<std::vector<Tower*>> towers;
    sf::RectangleShape overlay;
    sf::RectangleShape enterButton;
    sf::Text enterButtonText;
    sf::RectangleShape loadButton;       // New Load button
    sf::Text loadButtonText;
    sf::RectangleShape map1Button;       // Map selection buttons
    sf::Text map1ButtonText;
    sf::RectangleShape map2Button;
    sf::Text map2ButtonText;
    sf::RectangleShape map3Button;
    sf::Text map3ButtonText;
    sf::RectangleShape widthInputBox, heightInputBox;
    sf::Text widthInputText, heightInputText;
    sf::Text widthLabel, heightLabel;
    sf::Text enterSizeLabel;
    sf::Font font;
    std::string widthInput, heightInput;
    bool overlayActive;
    bool loadMenuActive;                 // Flag for showing map selection
    sf::RectangleShape startTile, endTile;
    sf::Texture grassTexture;
    sf::Texture pathTexture;
    sf::Texture directDamageTowerTexture, areaDamageTowerTexture, slowDamageTowerTexture;
    std::vector<sf::Vector2i> pathPositions;
    bool isDragging;
    sf::Vector2i startDragPos;
    int towerCount;
    const int maxTowers = 6;
    std::vector<MapObserver*> observers;
    int& playerCoins;

    void loadPresetMap(const std::string& filename); // New method to load preset map

public:
    Map(MapObserver& observer, int& coins);
    ~Map();
    void draw(sf::RenderWindow& window);
    void drawGrid(sf::RenderWindow& window);
    void drawOverlay(sf::RenderWindow& window);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void handleMouseDrag(sf::Event& event, sf::RenderWindow& window);
    void createGrid();
    void loadGrassTexture(const std::string& filePath);
    void loadPathTexture(const std::string& filePath);
    void loadTowerTextures();
    bool placeTower(int x, int y, TowerType type, float tileSize);
    bool canPlaceMoreTowers() const;
    bool isOverlayActive() const;
    bool isPathCreated() const;
    void handleRightClick(int x, int y);
    void upgradeTower(int gridX, int gridY);
    void sellTower(int gridX, int gridY);
    int getTowerCount() const;
    sf::Vector2f getStartPosition() const;
    const std::vector<sf::Vector2i>& getPathPositions() const;
    sf::Vector2f getTileSize() const;
    sf::Vector2f getEndPosition() const;
    const std::vector<std::vector<Tower*>>& getTowers() const;
    void updateGridDimensions(sf::RenderWindow& window);
    void attach(MapObserver* observer);
    void detach(MapObserver* observer);
    void notify(EventType event);
    void setPlayerCoins(int& coins);
    void resetMap();
    void clearPath();
    void resetOverlay();
    bool verifyPath() const;
};

#endif