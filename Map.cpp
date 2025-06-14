#include <queue>
#include "Map.h"
#include "Logger.h"
#include "MapObserver.h"
#include "DirectDamageTower.h"
#include "AreaDamageTower.h"
#include "SlowDamageTower.h"

using namespace std;

Map::Map(MapObserver& observer, int& coins) : width(10), height(10), overlayActive(true), loadMenuActive(false), widthInput("10"), heightInput("10"), isDragging(false), towerCount(0), playerCoins(coins) {
    observer.setSubject(this);
    attach(&observer);

    if (!font.loadFromFile("./assets/Arial.ttf")) {
        cout << ("Failed to load font") << endl;
    }

    overlay.setSize({800, 800});
    overlay.setFillColor(sf::Color(50, 50, 50, 200));
    overlay.setPosition(0, 0);

    enterButton.setSize({120, 60});
    enterButton.setFillColor(sf::Color(0, 150, 0));
    enterButton.setOutlineColor(sf::Color::White);
    enterButton.setOutlineThickness(2);
    enterButton.setPosition((800 - 120 * 2 - 40) / 2, 650);

    enterButtonText.setFont(font);
    enterButtonText.setCharacterSize(24);
    enterButtonText.setFillColor(sf::Color::White);
    enterButtonText.setString("Enter");
    sf::FloatRect enterBounds = enterButtonText.getLocalBounds();
    enterButtonText.setPosition(
        enterButton.getPosition().x + (120 - enterBounds.width) / 2 - enterBounds.left,
        650 + (60 - enterBounds.height) / 2 - enterBounds.top
    );

    loadButton.setSize({120, 60});
    loadButton.setFillColor(sf::Color(0, 0, 255));
    loadButton.setOutlineColor(sf::Color::White);
    loadButton.setOutlineThickness(2);
    loadButton.setPosition(enterButton.getPosition().x + 120 + 40, 650);

    loadButtonText.setFont(font);
    loadButtonText.setCharacterSize(24);
    loadButtonText.setFillColor(sf::Color::White);
    loadButtonText.setString("Load");
    sf::FloatRect loadBounds = loadButtonText.getLocalBounds();
    loadButtonText.setPosition(
        loadButton.getPosition().x + (120 - loadBounds.width) / 2 - loadBounds.left,
        650 + (60 - loadBounds.height) / 2 - loadBounds.top
    );

    // Center map buttons vertically and horizontally
    const int buttonHeight = 60;
    const int spacing = 80;
    const int totalHeight = (3 * buttonHeight) + (2 * spacing); // 3 buttons, 2 gaps
    const int startY = (800 - totalHeight) / 2; // Center vertically in 800px height

    map1Button.setSize({120, buttonHeight});
    map1Button.setFillColor(sf::Color(0, 150, 0));
    map1Button.setOutlineColor(sf::Color::White);
    map1Button.setOutlineThickness(2);
    map1Button.setPosition((800 - 120) / 2, startY);

    map1ButtonText.setFont(font);
    map1ButtonText.setCharacterSize(24);
    map1ButtonText.setFillColor(sf::Color::White);
    map1ButtonText.setString("Map 1");
    sf::FloatRect map1Bounds = map1ButtonText.getLocalBounds();
    map1ButtonText.setPosition(
        map1Button.getPosition().x + (120 - map1Bounds.width) / 2 - map1Bounds.left,
        startY + (buttonHeight - map1Bounds.height) / 2 - map1Bounds.top
    );

    map2Button.setSize({120, buttonHeight});
    map2Button.setFillColor(sf::Color(0, 150, 0));
    map2Button.setOutlineColor(sf::Color::White);
    map2Button.setOutlineThickness(2);
    map2Button.setPosition((800 - 120) / 2, startY + buttonHeight + spacing);

    map2ButtonText.setFont(font);
    map2ButtonText.setCharacterSize(24);
    map2ButtonText.setFillColor(sf::Color::White);
    map2ButtonText.setString("Map 2");
    sf::FloatRect map2Bounds = map2ButtonText.getLocalBounds();
    map2ButtonText.setPosition(
        map2Button.getPosition().x + (120 - map2Bounds.width) / 2 - map2Bounds.left,
        map2Button.getPosition().y + (buttonHeight - map2Bounds.height) / 2 - map2Bounds.top
    );

    map3Button.setSize({120, buttonHeight});
    map3Button.setFillColor(sf::Color(0, 150, 0));
    map3Button.setOutlineColor(sf::Color::White);
    map3Button.setOutlineThickness(2);
    map3Button.setPosition((800 - 120) / 2, startY + (buttonHeight + spacing) * 2);

    map3ButtonText.setFont(font);
    map3ButtonText.setCharacterSize(24);
    map3ButtonText.setFillColor(sf::Color::White);
    map3ButtonText.setString("Map 3");
    sf::FloatRect map3Bounds = map3ButtonText.getLocalBounds();
    map3ButtonText.setPosition(
        map3Button.getPosition().x + (120 - map3Bounds.width) / 2 - map3Bounds.left,
        map3Button.getPosition().y + (buttonHeight - map3Bounds.height) / 2 - map3Bounds.top
    );

    widthInputBox.setSize({150, 50});
    widthInputBox.setFillColor(sf::Color(220, 220, 220));
    widthInputBox.setOutlineColor(sf::Color::Black);
    widthInputBox.setOutlineThickness(1);
    widthInputBox.setPosition(200, 350);

    heightInputBox.setSize({150, 50});
    heightInputBox.setFillColor(sf::Color(220, 220, 220));
    heightInputBox.setOutlineColor(sf::Color::Black);
    heightInputBox.setOutlineThickness(1);
    heightInputBox.setPosition(450, 350);

    widthInputText.setFont(font);
    widthInputText.setCharacterSize(24);
    widthInputText.setFillColor(sf::Color::Black);
    widthInputText.setString(widthInput);
    sf::FloatRect widthTextBounds = widthInputText.getLocalBounds();
    widthInputText.setPosition(
        200 + (150 - widthTextBounds.width) / 2 - widthTextBounds.left,
        350 + (50 - widthTextBounds.height) / 2 - widthTextBounds.top
    );

    heightInputText.setFont(font);
    heightInputText.setCharacterSize(24);
    heightInputText.setFillColor(sf::Color::Black);
    heightInputText.setString(heightInput);
    sf::FloatRect heightTextBounds = heightInputText.getLocalBounds();
    heightInputText.setPosition(
        450 + (150 - heightTextBounds.width) / 2 - heightTextBounds.left,
        350 + (50 - heightTextBounds.height) / 2 - heightTextBounds.top
    );

    widthLabel.setFont(font);
    widthLabel.setCharacterSize(20);
    widthLabel.setFillColor(sf::Color::White);
    widthLabel.setString("Width:");
    sf::FloatRect widthLabelBounds = widthLabel.getLocalBounds();
    widthLabel.setPosition(
        200 + (150 - widthLabelBounds.width) / 2 - widthLabelBounds.left,
        310 - widthLabelBounds.top
    );

    heightLabel.setFont(font);
    heightLabel.setCharacterSize(20);
    heightLabel.setFillColor(sf::Color::White);
    heightLabel.setString("Height:");
    sf::FloatRect heightLabelBounds = heightLabel.getLocalBounds();
    heightLabel.setPosition(
        450 + (150 - heightLabelBounds.width) / 2 - heightLabelBounds.left,
        310 - heightLabelBounds.top
    );

    enterSizeLabel.setFont(font);
    enterSizeLabel.setCharacterSize(30);
    enterSizeLabel.setFillColor(sf::Color::White);
    enterSizeLabel.setString("Enter Map Size");
    sf::FloatRect titleBounds = enterSizeLabel.getLocalBounds();
    enterSizeLabel.setPosition(
        (800 - titleBounds.width) / 2 - titleBounds.left,
        200 - titleBounds.top
    );

    logObserver("[Map Observer]: Overlay loaded");
    notify(EventType::OverlayLoaded);

    loadGrassTexture("./assets/grass.png");
    loadPathTexture("./assets/path.png");
    loadTowerTextures();
    createGrid();

    logObserver("[Map Observer]: Map loaded");
    notify(EventType::MapLoaded);
}

Map::~Map() {
    for (auto& row : towers) {
        for (auto* tower : row) {
            delete tower;
        }
    }
}

void Map::drawOverlay(sf::RenderWindow& window) {
    window.draw(overlay);
    if (!loadMenuActive) {
        window.draw(enterButton);
        window.draw(enterButtonText);
        window.draw(loadButton);
        window.draw(loadButtonText);
        window.draw(widthInputBox);
        window.draw(heightInputBox);
        window.draw(widthInputText);
        window.draw(heightInputText);
        window.draw(widthLabel);
        window.draw(heightLabel);
        window.draw(enterSizeLabel);
    } else {
        window.draw(map1Button);
        window.draw(map1ButtonText);
        window.draw(map2Button);
        window.draw(map2ButtonText);
        window.draw(map3Button);
        window.draw(map3ButtonText);
    }
}

void Map::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (overlayActive) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f clickPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            cout << ("Mouse click at: (" + std::to_string(clickPos.x) + ", " + std::to_string(clickPos.y) + ")") << endl;
            notify(EventType::OverlayClicked);
            if (enterButton.getGlobalBounds().contains(clickPos) && !loadMenuActive) {
                cout << ("Enter button clicked") << endl;
                width = std::stoi(widthInput);
                height = std::stoi(heightInput);
                createGrid();
                overlayActive = false;
                cout << ("Map dimensions confirmed") << endl;
                notify(EventType::DimensionsConfirmed);
            } else if (loadButton.getGlobalBounds().contains(clickPos) && !loadMenuActive) {
                cout << ("Load button clicked") << endl;
                loadMenuActive = true;
            } else if (loadMenuActive) {
                if (map1Button.getGlobalBounds().contains(clickPos)) {
                    loadPresetMap("./assets/map1.txt");
                    overlayActive = false;
                    loadMenuActive = false;
                    notify(EventType::DimensionsConfirmed);
                } else if (map2Button.getGlobalBounds().contains(clickPos)) {
                    loadPresetMap("./assets/map2.txt");
                    overlayActive = false;
                    loadMenuActive = false;
                    notify(EventType::DimensionsConfirmed);
                } else if (map3Button.getGlobalBounds().contains(clickPos)) {
                    loadPresetMap("./assets/map3.txt");
                    overlayActive = false;
                    loadMenuActive = false;
                    notify(EventType::DimensionsConfirmed);
                }
            } else if (widthInputBox.getGlobalBounds().contains(clickPos)) {
                widthInput.clear();
                widthInputText.setString(widthInput);
                sf::FloatRect widthTextBounds = widthInputText.getLocalBounds();
                widthInputText.setPosition(
                    200 + (150 - widthTextBounds.width) / 2 - widthTextBounds.left,
                    350 + (50 - widthTextBounds.height) / 2 - widthTextBounds.top
                );
            } else if (heightInputBox.getGlobalBounds().contains(clickPos)) {
                heightInput.clear();
                heightInputText.setString(heightInput);
                sf::FloatRect heightTextBounds = heightInputText.getLocalBounds();
                heightInputText.setPosition(
                    450 + (150 - heightTextBounds.width) / 2 - heightTextBounds.left,
                    350 + (50 - heightTextBounds.height) / 2 - heightTextBounds.top
                );
            }
        } else if (event.type == sf::Event::TextEntered && !loadMenuActive) {
            cout << ("Text entered: " + std::string(1, static_cast<char>(event.text.unicode))) << endl;
            notify(EventType::OverlayClicked);
            if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                if (widthInputBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    widthInput += static_cast<char>(event.text.unicode);
                    widthInputText.setString(widthInput);
                    sf::FloatRect widthTextBounds = widthInputText.getLocalBounds();
                    widthInputText.setPosition(
                        200 + (150 - widthTextBounds.width) / 2 - widthTextBounds.left,
                        350 + (50 - widthTextBounds.height) / 2 - widthTextBounds.top
                    );
                } else if (heightInputBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    heightInput += static_cast<char>(event.text.unicode);
                    heightInputText.setString(heightInput);
                    sf::FloatRect heightTextBounds = heightInputText.getLocalBounds();
                    heightInputText.setPosition(
                        450 + (150 - heightTextBounds.width) / 2 - heightTextBounds.left,
                        350 + (50 - heightTextBounds.height) / 2 - heightTextBounds.top
                    );
                }
            } else if (event.text.unicode == 8) {
                if (widthInputBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    if (!widthInput.empty()) widthInput.pop_back();
                    widthInputText.setString(widthInput);
                    sf::FloatRect widthTextBounds = widthInputText.getLocalBounds();
                    widthInputText.setPosition(
                        200 + (150 - widthTextBounds.width) / 2 - widthTextBounds.left,
                        350 + (50 - widthTextBounds.height) / 2 - widthTextBounds.top
                    );
                } else if (heightInputBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    if (!heightInput.empty()) heightInput.pop_back();
                    heightInputText.setString(heightInput);
                    sf::FloatRect heightTextBounds = heightInputText.getLocalBounds();
                    heightInputText.setPosition(
                        450 + (150 - heightTextBounds.width) / 2 - heightTextBounds.left,
                        350 + (50 - heightTextBounds.height) / 2 - heightTextBounds.top
                    );
                }
            }
        }
    } else {
        handleMouseDrag(event, window);
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            handleRightClick(mousePos.x, mousePos.y);
        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    drawGrid(window);
    if (overlayActive) drawOverlay(window);
    for (const auto& row : towers) {
        for (const auto* tower : row) {
            if (tower) tower->draw(window);
        }
    }
}

bool Map::isOverlayActive() const { return overlayActive; }

void Map::createGrid() {
    cout << "Creating grid with dimensions: " << std::to_string(width) << "x" << std::to_string(height) << endl;
    grid.clear();
    grid.resize(height, std::vector<sf::RectangleShape>(width));
    towers.clear();
    towers.resize(height, std::vector<Tower*>(width, nullptr));
    sf::Vector2f overlaySize = overlay.getSize();
    sf::Vector2u windowSize(static_cast<unsigned int>(overlaySize.x), static_cast<unsigned int>(overlaySize.y));
    float tileSize = std::min(static_cast<float>(windowSize.x) / width, static_cast<float>(windowSize.y) / height);
    sf::Vector2u grassTextureSize = grassTexture.getSize();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            grid[i][j].setSize({tileSize, tileSize});
            grid[i][j].setTexture(&grassTexture);
            grid[i][j].setTextureRect(sf::IntRect(0, 0, grassTextureSize.x, grassTextureSize.y));
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1);
            grid[i][j].setPosition(j * tileSize, i * tileSize);
        }
    }
    startTile.setSize({tileSize, tileSize});
    startTile.setFillColor(sf::Color::Green);
    startTile.setPosition(0, (height / 2 - 1) * tileSize);
    endTile.setSize({tileSize, tileSize});
    endTile.setFillColor(sf::Color::Red);
    endTile.setPosition((width - 1) * tileSize, (height / 2) * tileSize);
}

void Map::drawGrid(sf::RenderWindow& window) {
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            window.draw(cell);
        }
    }
    window.draw(startTile);
    window.draw(endTile);
}

void Map::loadGrassTexture(const std::string& filePath) {
    if (!grassTexture.loadFromFile(filePath)) logObserver("Failed to load grass texture");
}

void Map::loadPathTexture(const std::string& filePath) {
    if (!pathTexture.loadFromFile(filePath)) logObserver("Failed to load path texture");
}

void Map::loadTowerTextures() {
    if (!directDamageTowerTexture.loadFromFile("./assets/direct_tower.png")) logObserver("Failed to load direct damage tower texture");
    if (!areaDamageTowerTexture.loadFromFile("./assets/area_tower.png")) logObserver("Failed to load area damage tower texture");
    if (!slowDamageTowerTexture.loadFromFile("./assets/slow_tower.png")) logObserver("Failed to load slow damage tower texture");
}

void Map::handleMouseDrag(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2i endTilePos(endTile.getPosition().x / grid[0][0].getSize().x, endTile.getPosition().y / grid[0][0].getSize().y);
        if (startTile.getGlobalBounds().contains(mousePos) ||
            (!pathPositions.empty() && grid[pathPositions.back().y][pathPositions.back().x].getGlobalBounds().contains(mousePos) &&
             pathPositions.back() != endTilePos)) {
            isDragging = true;
            startDragPos = sf::Vector2i(mousePos.x / grid[0][0].getSize().x, mousePos.y / grid[0][0].getSize().y);
            if (pathPositions.empty()) pathPositions.push_back(startDragPos);
        }
    } else if (event.type == sf::Event::MouseMoved && isDragging && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2i currentPos(mousePos.x / grid[0][0].getSize().x, mousePos.y / grid[0][0].getSize().y);
        if (currentPos.x >= 0 && currentPos.x < width && currentPos.y >= 0 && currentPos.y < height) {
            if (currentPos != pathPositions.back()) {
                int dx = std::abs(currentPos.x - pathPositions.back().x);
                int dy = std::abs(currentPos.y - pathPositions.back().y);
                if (dx <= 1 && dy <= 1 && (dx + dy > 0)) {
                    int minX = std::min(pathPositions.back().x, currentPos.x);
                    int maxX = std::max(pathPositions.back().x, currentPos.x);
                    int minY = std::min(pathPositions.back().y, currentPos.y);
                    int maxY = std::max(pathPositions.back().y, currentPos.y);
                    for (int y = minY; y <= maxY; ++y) {
                        for (int x = minX; x <= maxX; ++x) {
                            if (x >= 0 && x < width && y >= 0 && y < height) {
                                grid[y][x].setTexture(&pathTexture);
                                grid[y][x].setTextureRect(sf::IntRect(0, 0, pathTexture.getSize().x, pathTexture.getSize().y));
                                pathPositions.push_back(sf::Vector2i(x, y));
                            }
                        }
                    }
                    pathPositions.push_back(currentPos);
                }
            }
        }
    } else if (event.type == sf::Event::MouseButtonReleased && isDragging) {
        isDragging = false;
        notify(EventType::PathDraw);
        if (isPathCreated()) {
            std::vector<sf::Vector2i> cleanedPath;
            cleanedPath.reserve(pathPositions.size());
            for (const auto& pos : pathPositions) {
                bool isDuplicate = false;
                for (const auto& seenPos : cleanedPath) {
                    if (seenPos == pos) {
                        isDuplicate = true;
                        break;
                    }
                }
                if (!isDuplicate) {
                    cleanedPath.push_back(pos);
                }
            }
            pathPositions = std::move(cleanedPath);

            cout << "Path coordinates (duplicates removed):" << endl;
            for (const auto& pos : pathPositions) {
                cout << "(" << pos.x << ", " << pos.y << ")" << endl;
            }
            cout << "Path starts at (" << pathPositions[0].x << ", " << pathPositions[0].y << ")" << endl;
            cout << "Path creation completed." << endl;

            if (verifyPath()) {
                cout << "Path is valid: Connected from start to end." << endl;
                logObserver("[Map Observer]: Path verified as valid");
            }
            else {
                cout << "Path is invalid: No continuous path from start to end." << endl;
                logObserver("[Map Observer]: Path verified as invalid");
                // Optionally clear the path for retry
                clearPath();
                cout << "Path cleared. Please try again." << endl;
            }


        }
    }
}

void Map::updateGridDimensions(sf::RenderWindow& window) {
    createGrid();
}

bool Map::placeTower(int x, int y, TowerType type, float tileSize) {
    if (towerCount >= maxTowers) return false;
    int gridX = x / grid[0][0].getSize().x;
    int gridY = y / grid[0][0].getSize().y;
    if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
        for (const auto& pos : pathPositions) {
            if (pos.x == gridX && pos.y == gridY) return false;
        }
        sf::Vector2f position(gridX * getTileSize().x, gridY * getTileSize().y);
        delete towers[gridY][gridX];
        switch (type) {
            case TowerType::DirectDamage:
                towers[gridY][gridX] = new DirectDamageTower(position);
                break;
            case TowerType::AreaDamage:
                towers[gridY][gridX] = new AreaDamageTower(position);
                break;
            case TowerType::SlowDamage:
                towers[gridY][gridX] = new SlowDamageTower(position);
                break;
        }
        towers[gridY][gridX]->setScaleToTileSize(tileSize);
        towerCount++;
        cout << "Tower placed at (" << std::to_string(gridX) << ", " << std::to_string(gridY) << ")" << endl;
        notify(EventType::TowerPlaced);
        return true;
    }
    return false;
}

bool Map::canPlaceMoreTowers() const { return towerCount < maxTowers; }

bool Map::isPathCreated() const {
    return !pathPositions.empty() &&
           pathPositions.back() == sf::Vector2i(endTile.getPosition().x / grid[0][0].getSize().x,
                                                endTile.getPosition().y / grid[0][0].getSize().y);
}

void Map::handleRightClick(int x, int y) {
    int gridX = x / grid[0][0].getSize().x;
    int gridY = y / grid[0][0].getSize().y;
    if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
        if (towers[gridY][gridX]) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) upgradeTower(gridX, gridY);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) sellTower(gridX, gridY);
        }
    }
}

void Map::upgradeTower(int gridX, int gridY) {
    if (towers[gridY][gridX]) {
        towers[gridY][gridX]->upgrade(playerCoins);
        logObserver("[TowerObserver]: Tower upgraded to level " + std::to_string(towers[gridY][gridX]->getLevel()));
    }
}

void Map::sellTower(int gridX, int gridY) {
    if (towers[gridY][gridX]) {
        towers[gridY][gridX]->sell(playerCoins);
        delete towers[gridY][gridX];
        towers[gridY][gridX] = nullptr;
        towerCount--;
        logObserver("[TowerObserver]: Tower sold");
    }
}

int Map::getTowerCount() const { return towerCount; }

sf::Vector2f Map::getStartPosition() const { return startTile.getPosition(); }

const std::vector<sf::Vector2i>& Map::getPathPositions() const { return pathPositions; }

sf::Vector2f Map::getTileSize() const { return grid.empty() ? sf::Vector2f(0, 0) : grid[0][0].getSize(); }

sf::Vector2f Map::getEndPosition() const { return endTile.getPosition(); }

const std::vector<std::vector<Tower*>>& Map::getTowers() const { return towers; }

void Map::attach(MapObserver* observer) {
    if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) {
        observers.push_back(observer);
        logObserver("[Map Observer]: Observer attached. Total observers: " + std::to_string(observers.size()));
    }
}

void Map::detach(MapObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    logObserver("[Map Observer]: Observer detached. Total observers: " + std::to_string(observers.size()));
}

void Map::notify(EventType event) {
    std::string eventStr;
    switch (event) {
        case EventType::MapLoaded: eventStr = "MapLoaded"; break;
        case EventType::OverlayLoaded: eventStr = "OverlayLoaded"; break;
        case EventType::OverlayClicked: eventStr = "OverlayClicked"; break;
        case EventType::DimensionsConfirmed: eventStr = "DimensionsConfirmed"; break;
        case EventType::PathDraw: eventStr = "PathDraw"; break;
        case EventType::TowerPlaced: eventStr = "TowerPlaced"; break;
    }
    logObserver("[Map Observer]: Notifying observers of event: " + eventStr);
    for (auto* observer : observers) {
        observer->update();
    }
}

void Map::setPlayerCoins(int& coins) {
    playerCoins = coins;
}

void Map::resetMap() {
    for (auto& row : towers) {
        for (auto* tower : row) {
            delete tower;
        }
        row.assign(width, nullptr);
    }
    towerCount = 0;
}

void Map::clearPath() {
    pathPositions.clear();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            grid[i][j].setTexture(&grassTexture);
            grid[i][j].setTextureRect(sf::IntRect(0, 0, grassTexture.getSize().x, grassTexture.getSize().y));
        }
    }
}

void Map::resetOverlay() {
    overlayActive = true;
    loadMenuActive = false;
    widthInput = "10";
    heightInput = "10";
    widthInputText.setString(widthInput);
    heightInputText.setString(heightInput);
    sf::FloatRect widthTextBounds = widthInputText.getLocalBounds();
    widthInputText.setPosition(
        200 + (150 - widthTextBounds.width) / 2 - widthTextBounds.left,
        350 + (50 - widthTextBounds.height) / 2 - widthTextBounds.top
    );
    sf::FloatRect heightTextBounds = heightInputText.getLocalBounds();
    heightInputText.setPosition(
        450 + (150 - heightTextBounds.width) / 2 - heightTextBounds.left,
        350 + (50 - heightTextBounds.height) / 2 - heightTextBounds.top
    );
}

void Map::loadPresetMap(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        logObserver("Failed to open preset map file: " + filename);
        return;
    }

    file >> width >> height;
    createGrid();

    int startX, startY, endX, endY;
    file >> startX >> startY >> endX >> endY;
    float tileSize = getTileSize().x;
    startTile.setPosition(startX * tileSize, startY * tileSize);
    endTile.setPosition(endX * tileSize, endY * tileSize);

    pathPositions.clear();
    int x, y;
    while (file >> x >> y) {
        pathPositions.push_back(sf::Vector2i(x, y));
        grid[y][x].setTexture(&pathTexture);
        grid[y][x].setTextureRect(sf::IntRect(0, 0, pathTexture.getSize().x, pathTexture.getSize().y));
    }

    file.close();
    logObserver("Loaded preset map from: " + filename);
}

bool Map::verifyPath() const {
    if (pathPositions.empty()) return false;

    // Get start and end positions in grid coordinates
    sf::Vector2i startPos(
        static_cast<int>(startTile.getPosition().x / getTileSize().x),
        static_cast<int>(startTile.getPosition().y / getTileSize().y)
    );
    sf::Vector2i endPos(
        static_cast<int>(endTile.getPosition().x / getTileSize().x),
        static_cast<int>(endTile.getPosition().y / getTileSize().y)
    );

    // Check if start and end are in pathPositions
    bool startFound = false, endFound = false;
    for (const auto& pos : pathPositions) {
        if (pos == startPos) startFound = true;
        if (pos == endPos) endFound = true;
    }
    if (!startFound || !endFound) return false;

    // BFS 
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::queue<sf::Vector2i> q;
    q.push(startPos);
    visited[startPos.y][startPos.x] = true;

    // Directions for adjacent tiles (up, right, down, left)
    const sf::Vector2i directions[] = {
        {0, -1}, {1, 0}, {0, 1}, {-1, 0}
    };

    while (!q.empty()) {
        sf::Vector2i current = q.front();
        q.pop();

        if (current == endPos) return true;

        for (const auto& dir : directions) {
            sf::Vector2i next = current + dir;
            if (next.x >= 0 && next.x < width && next.y >= 0 && next.y < height && !visited[next.y][next.x]) {
                // Check if the next position is part of the path
                bool isPathTile = false;
                for (const auto& pos : pathPositions) {
                    if (pos == next) {
                        isPathTile = true;
                        break;
                    }
                }
                if (isPathTile) {
                    visited[next.y][next.x] = true;
                    q.push(next);
                }
            }
        }
    }

    return false; // No path found
}