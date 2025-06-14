#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Map.h"
#include "ConcreteMapObserver.h"
#include "CritterView.h"
#include "CritterGroupGenerator.h"
#include "Bullet.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include "Logger.h"

using namespace std;

ofstream logFile;

int crittersKilled = 0;
bool gameOver = false;
enum class GameAction { 
    None, 
    Exit, 
    Restart 
};

void logObserver(const string& message) {
    if (logFile.is_open()) {
        logFile << message << endl;
    }
}

TowerType selectedTowerType = TowerType::DirectDamage;

void drawTowerSelection(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape directDamageButton(sf::Vector2f(200, 50));
    directDamageButton.setFillColor(sf::Color(100, 100, 100));
    directDamageButton.setOutlineColor(sf::Color::White);
    directDamageButton.setOutlineThickness(2);
    directDamageButton.setPosition(50, 820);

    sf::Text directDamageText;
    directDamageText.setFont(font);
    directDamageText.setCharacterSize(20);
    directDamageText.setFillColor(sf::Color::White);
    directDamageText.setString("Direct Damage");
    sf::FloatRect directDamageTextBounds = directDamageText.getLocalBounds();
    directDamageText.setPosition(50 + (200 - directDamageTextBounds.width) / 2 - directDamageTextBounds.left, 
                                 820 + (50 - directDamageTextBounds.height) / 2 - directDamageTextBounds.top);

    sf::RectangleShape areaDamageButton(sf::Vector2f(200, 50));
    areaDamageButton.setFillColor(sf::Color(100, 100, 100));
    areaDamageButton.setOutlineColor(sf::Color::White);
    areaDamageButton.setOutlineThickness(2);
    areaDamageButton.setPosition(300, 820);

    sf::Text areaDamageText;
    areaDamageText.setFont(font);
    areaDamageText.setCharacterSize(20);
    areaDamageText.setFillColor(sf::Color::White);
    areaDamageText.setString("Area Damage");
    sf::FloatRect areaDamageTextBounds = areaDamageText.getLocalBounds();
    areaDamageText.setPosition(300 + (200 - areaDamageTextBounds.width) / 2 - areaDamageTextBounds.left, 
                               820 + (50 - areaDamageTextBounds.height) / 2 - areaDamageTextBounds.top);

    sf::RectangleShape slowDamageButton(sf::Vector2f(200, 50));
    slowDamageButton.setFillColor(sf::Color(100, 100, 100));
    slowDamageButton.setOutlineColor(sf::Color::White);
    slowDamageButton.setOutlineThickness(2);
    slowDamageButton.setPosition(550, 820);

    sf::Text slowDamageText;
    slowDamageText.setFont(font);
    slowDamageText.setCharacterSize(20);
    slowDamageText.setFillColor(sf::Color::White);
    slowDamageText.setString("Slow Damage");
    sf::FloatRect slowDamageTextBounds = slowDamageText.getLocalBounds();
    slowDamageText.setPosition(550 + (200 - slowDamageTextBounds.width) / 2 - slowDamageTextBounds.left, 
                               820 + (50 - slowDamageTextBounds.height) / 2 - slowDamageTextBounds.top);

    window.draw(directDamageButton);
    window.draw(directDamageText);
    window.draw(areaDamageButton);
    window.draw(areaDamageText);
    window.draw(slowDamageButton);
    window.draw(slowDamageText);
}

void handleTowerSelection(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (mousePos.y >= 820 && mousePos.y <= 870) {
            if (mousePos.x >= 50 && mousePos.x <= 250) {
                selectedTowerType = TowerType::DirectDamage;
                cout << "Tower selection: Direct Damage" << endl;
            } else if (mousePos.x >= 300 && mousePos.x <= 500) {
                selectedTowerType = TowerType::AreaDamage;
                cout << "Tower selection: Area Damage" << endl;
            } else if (mousePos.x >= 550 && mousePos.x <= 750) {
                selectedTowerType = TowerType::SlowDamage;
                cout << "Tower selection: Slow Damage" << endl;
            }
        }
    }
}

void promptUserToPlaceTowers(Map& map, sf::RenderWindow& window, sf::Font& font, bool& towersPlaced, bool forcePlacement = false) {
    if (towersPlaced && !forcePlacement) return;
    int targetCount = forcePlacement ? map.getTowerCount() + 1 : 6;
    while (map.getTowerCount() < targetCount) {
        bool towerPlaced = false;
        while (!towerPlaced) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
                handleTowerSelection(window, event);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (mousePos.y < 800) {
                        float tileSize = map.getTileSize().x;
                        towerPlaced = map.placeTower(mousePos.x, mousePos.y, selectedTowerType, tileSize);
                    }
                }
            }
            window.clear();
            map.draw(window);
            drawTowerSelection(window, font);
            window.display();
        }
    }
    if (!forcePlacement) towersPlaced = true;
}

bool showQuitConfirmation(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape dialog(sf::Vector2f(300, 150));
    dialog.setFillColor(sf::Color(50, 50, 50, 200));
    dialog.setPosition(250, 325);

    sf::Text promptText;
    promptText.setFont(font);
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setString("Quit Game?\n1. Quit\n2. Resume");
    sf::FloatRect textBounds = promptText.getLocalBounds();
    promptText.setPosition(250 + (300 - textBounds.width) / 2 - textBounds.left, 
                           325 + (150 - textBounds.height) / 2 - textBounds.top);

    sf::RenderWindow confirmWindow(sf::VideoMode(800, 800), "Confirm Quit", sf::Style::None);
    confirmWindow.setPosition(window.getPosition());

    while (confirmWindow.isOpen()) {
        sf::Event event;
        while (confirmWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) confirmWindow.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    confirmWindow.close();
                    cout << "User chose to quit the game" << endl;
                    return true;
                } else if (event.key.code == sf::Keyboard::Num2) {
                    confirmWindow.close();
                    cout << "User chose to resume the game" << endl;
                    return false;
                }
            }
        }
        confirmWindow.clear(sf::Color::Transparent);
        confirmWindow.draw(dialog);
        confirmWindow.draw(promptText);
        confirmWindow.display();
    }
    return false;
}

GameAction displayGameOverOverlay(sf::RenderWindow& window, int playerCoins, int crittersKilled, const sf::Font& font) {
    sf::RectangleShape overlay(sf::Vector2f(800, 900));
    overlay.setFillColor(sf::Color(50, 50, 50, 200));
    overlay.setPosition(0, 0);

    sf::Text gameOverText;
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setFont(font);
    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition((800 - gameOverBounds.width) / 2 - gameOverBounds.left, 200 - gameOverBounds.top);

    sf::Text coinsText;
    coinsText.setString("Coins: " + std::to_string(playerCoins));
    coinsText.setCharacterSize(20);
    coinsText.setFillColor(sf::Color::White);
    coinsText.setFont(font);
    sf::FloatRect coinsBounds = coinsText.getLocalBounds();
    coinsText.setPosition((800 - coinsBounds.width) / 2 - coinsBounds.left, 300 - coinsBounds.top);

    sf::Text killedText;
    killedText.setString("Critters Killed: " + std::to_string(crittersKilled));
    killedText.setCharacterSize(20);
    killedText.setFillColor(sf::Color::White);
    killedText.setFont(font);
    sf::FloatRect killedBounds = killedText.getLocalBounds();
    killedText.setPosition((800 - killedBounds.width) / 2 - killedBounds.left, 350 - killedBounds.top);

    sf::RectangleShape exitButton(sf::Vector2f(100, 40));
    exitButton.setFillColor(sf::Color(200, 50, 50));
    exitButton.setOutlineColor(sf::Color::White);
    exitButton.setOutlineThickness(2);
    exitButton.setPosition(300, 450);

    sf::Text exitText;
    exitText.setString("Exit");
    exitText.setCharacterSize(20);
    exitText.setFillColor(sf::Color::White);
    exitText.setFont(font);
    sf::FloatRect exitTextBounds = exitText.getLocalBounds();
    exitText.setPosition(300 + (100 - exitTextBounds.width) / 2 - exitTextBounds.left, 
                         450 + (40 - exitTextBounds.height) / 2 - exitTextBounds.top);

    sf::RectangleShape restartButton(sf::Vector2f(100, 40));
    restartButton.setFillColor(sf::Color(0, 150, 0));
    restartButton.setOutlineColor(sf::Color::White);
    restartButton.setOutlineThickness(2);
    restartButton.setPosition(450, 450);

    sf::Text restartText;
    restartText.setString("Restart");
    restartText.setCharacterSize(20);
    restartText.setFillColor(sf::Color::White);
    restartText.setFont(font);
    sf::FloatRect restartTextBounds = restartText.getLocalBounds();
    restartText.setPosition(450 + (100 - restartTextBounds.width) / 2 - restartTextBounds.left, 
                            450 + (40 - restartTextBounds.height) / 2 - restartTextBounds.top);

    while (true) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return GameAction::Exit;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (exitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    return GameAction::Exit;
                } else if (restartButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    return GameAction::Restart;
                }
            }
        }
        window.clear();
        window.draw(overlay);
        window.draw(gameOverText);
        window.draw(coinsText);
        window.draw(killedText);
        window.draw(exitButton);
        window.draw(exitText);
        window.draw(restartButton);
        window.draw(restartText);
        window.display();
    }
}

void resetGame(vector<Critter*>& critters, int& playerCoins, int& waveNumber, int& crittersKilled, bool& towersPlaced, Map& map, sf::Clock& waveTimer) {
    for (auto* critter : critters) {
        delete critter;
    }
    critters.clear();
    playerCoins = 1000;
    waveNumber = 1;
    crittersKilled = 0;
    towersPlaced = false;
    map.resetMap();
    map.clearPath();
    map.resetOverlay();
    waveTimer.restart();
}

int main() {
    logFile.open("observer_log.txt", ios::out | ios::trunc);
    if (!logFile.is_open()) {
        cerr << "Error: Could not open log file!" << endl;
        return 1;
    }
    logObserver("Program started");

    sf::RenderWindow window(sf::VideoMode(800, 900), "Tower Defense", sf::Style::Titlebar | sf::Style::Close);
    ConcreteMapObserver observer;
    int playerCoins = 1000;
    Map map(observer, playerCoins);
    bool towersPlaced = false;

    sf::Font font;
    if (!font.loadFromFile("./assets/Arial.ttf")) cout << ("Failed to load font") << endl;
    sf::Text coinText;
    coinText.setFont(font);
    coinText.setCharacterSize(20);
    coinText.setFillColor(sf::Color::White);
    coinText.setPosition(10, 10);

    sf::Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(20);
    messageText.setFillColor(sf::Color::Yellow);
    messageText.setPosition(10, 40);
    sf::Clock messageTimer;
    bool isSellOrUpgradeMessage = false; // Flag to differentiate timed messages

    sf::RectangleShape quitButton(sf::Vector2f(100, 40));
    quitButton.setFillColor(sf::Color(200, 50, 50));
    quitButton.setOutlineColor(sf::Color::White);
    quitButton.setOutlineThickness(2);
    quitButton.setPosition(690, 10);

    sf::Text quitText;
    quitText.setFont(font);
    quitText.setCharacterSize(20);
    quitText.setFillColor(sf::Color::White);
    quitText.setString("Quit");
    sf::FloatRect quitTextBounds = quitText.getLocalBounds();
    quitText.setPosition(690 + (100 - quitTextBounds.width) / 2 - quitTextBounds.left, 
                         10 + (40 - quitTextBounds.height) / 2 - quitTextBounds.top);

    map.loadGrassTexture("./assets/grass.png");
    map.updateGridDimensions(window);

    CritterGroupGenerator critterGen;
    std::vector<Critter*> critters;
    std::vector<Bullet> bullets;
    CritterView critterView(window);
    int waveNumber = 1;
    sf::Clock waveTimer;
    sf::Clock frameClock;
    bool firstMoveLogged = false;
    bool pathValid = false;
    bool validation = true;

    while (window.isOpen()) {
        if (gameOver) {
            GameAction action = displayGameOverOverlay(window, playerCoins, crittersKilled, font);
            if (action == GameAction::Exit) {
                window.close();
            } else if (action == GameAction::Restart) {
                resetGame(critters, playerCoins, waveNumber, crittersKilled, towersPlaced, map, waveTimer);
                gameOver = false;
                firstMoveLogged = false;
            }
        } else {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
                if (map.isOverlayActive()) {
                    map.handleEvent(event, window);
                } else {
                    map.handleMouseDrag(event, window);
                    handleTowerSelection(window, event);
                    if (event.type == sf::Event::MouseButtonReleased && validation) {
                        pathValid = map.verifyPath(); // Update path validity
                        if (pathValid) {
                            messageText.setString("Path is valid!");
                            messageText.setFillColor(sf::Color::Green);
                            validation = false;
                        }
                        else {
                            messageText.setString("Path is invalid! Retry.");
                            messageText.setFillColor(sf::Color::Red);
                        }
                        messageTimer.restart();
                    }
                    if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (event.mouseButton.button == sf::Mouse::Right) {
                            int prevCoins = playerCoins;
                            int prevTowerCount = map.getTowerCount();
                            map.handleRightClick(mousePos.x, mousePos.y);
                            if (playerCoins > prevCoins) {
                                const auto& towers = map.getTowers();
                                for (int y = 0; y < towers.size(); ++y) {
                                    for (int x = 0; x < towers[y].size(); ++x) {
                                        if (!towers[y][x] && prevTowerCount > map.getTowerCount()) {
                                            messageText.setString("Tower sold");
                                            messageTimer.restart();
                                            isSellOrUpgradeMessage = true;
                                            promptUserToPlaceTowers(map, window, font, towersPlaced, true);
                                            break;
                                        }
                                    }
                                }
                            } else if (playerCoins < prevCoins) {
                                int gridX = mousePos.x / map.getTileSize().x;
                                int gridY = mousePos.y / map.getTileSize().y;
                                if (gridX >= 0 && gridX < map.getTowers()[0].size() && gridY >= 0 && gridY < map.getTowers().size() && map.getTowers()[gridY][gridX]) {
                                    messageText.setString(map.getTowers()[gridY][gridX]->getName() + " upgraded to level " + std::to_string(map.getTowers()[gridY][gridX]->getLevel()));
                                    messageTimer.restart();
                                    isSellOrUpgradeMessage = true;
                                }
                            }
                        } else if (event.mouseButton.button == sf::Mouse::Left && quitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            if (showQuitConfirmation(window, font)) {
                                window.close();
                            }
                        }
                    }
                }
            }

            if (!map.isOverlayActive()) {
                // Check path validity during drawing
                if (!map.isPathCreated() && !map.getPathPositions().empty()) {
                    messageText.setString("Invalid Path");
                    isSellOrUpgradeMessage = false; // Path message doesn't use timer
                } else if (!isSellOrUpgradeMessage) {
                    messageText.setString(""); // Clear path message when complete
                }
            }

            if (!map.isOverlayActive() && map.isPathCreated()) {
                if (critters.empty() && waveTimer.getElapsedTime().asSeconds() > 10) {
                    for (auto* critter : critters) delete critter;
                    critters.clear();
                    critters = critterGen.generateWave(waveNumber++, map.getStartPosition(), map.getTileSize().x);
                    cout << "Wave " + std::to_string(waveNumber) + " generated with " + std::to_string(critters.size()) + " critters" << endl;
                    cout << "Start tile position: (" << map.getStartPosition().x << ", " << map.getStartPosition().y << ")" << endl;
                    for (size_t i = 0; i < critters.size(); ++i) {
                        cout << "Critter " << i + 1 << " spawned at (" << critters[i]->getPosition().x << ", " << critters[i]->getPosition().y << ")" << endl;
                        critterView.addCritter(critters[i]);
                    }
                    waveTimer.restart();
                    firstMoveLogged = false;
                }

                float deltaTime = frameClock.restart().asSeconds();
                sf::Vector2f endTileCenter(
                    map.getEndPosition().x + map.getTileSize().x / 2.0f,
                    map.getEndPosition().y + map.getTileSize().y / 2.0f
                );

                for (auto it = critters.begin(); it != critters.end();) {
                    (*it)->move(map.getPathPositions(), map.getTileSize().x, deltaTime);
                    if (!firstMoveLogged && !critters.empty()) {
                        cout << "After first move, Critter 1 at (" << critters[0]->getPosition().x << ", " << critters[0]->getPosition().y << ")" << endl;
                        firstMoveLogged = true;
                    }
                    if ((*it)->isDead()) {
                        playerCoins += (*it)->getReward();
                        critterView.removeCritter(*it);
                        delete *it;
                        it = critters.erase(it);
                        crittersKilled++;
                    } else if ((*it)->hasReachedExit(endTileCenter)) {
                        playerCoins -= (*it)->getStrength();
                        logObserver("Critter reached exit, player lost " + std::to_string((*it)->getStrength()) + " coins");
                        critterView.removeCritter(*it);
                        delete *it;
                        it = critters.erase(it);
                        gameOver = true;
                    } else {
                        ++it;
                    }
                }

                for (auto it = bullets.begin(); it != bullets.end();) {
                    it->update(deltaTime);
                    if (!it->isActive()) {
                        it = bullets.erase(it);
                    } else {
                        ++it;
                    }
                }

                const auto& towers = map.getTowers();
                for (int y = 0; y < towers.size(); ++y) {
                    for (int x = 0; x < towers[y].size(); ++x) {
                        if (towers[y][x] && towers[y][x]->getLevel() > 0) {
                            towers[y][x]->attack(critters, bullets);
                            towers[y][x]->updateAnimation();
                        }
                    }
                }

                promptUserToPlaceTowers(map, window, font, towersPlaced);
            }

            coinText.setString("Coins: " + std::to_string(playerCoins));
            if (isSellOrUpgradeMessage && messageTimer.getElapsedTime().asSeconds() > 2.0f) {
                messageText.setString("");
                isSellOrUpgradeMessage = false;
            }

            window.clear();
            map.draw(window);
            for (auto* critter : critters) {
                if (critter) critter->draw(window);
            }
            for (const auto& bullet : bullets) {
                bullet.draw(window);
            }
            window.draw(coinText);
            window.draw(messageText);
            window.draw(quitButton);
            window.draw(quitText);
            drawTowerSelection(window, font);
            window.display();
        }
    }

    for (auto* critter : critters) delete critter;
    critters.clear();

    logObserver("Program ended");
    logFile.close();

    return 0;
}