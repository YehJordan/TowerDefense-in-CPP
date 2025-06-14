#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Bullet {
public:
    Bullet(sf::Vector2f start, sf::Vector2f target, float speed = 200.0f)
        : position(start), target(target), speed(speed), active(true) {
        shape.setRadius(2.0f); // Small dot for "."
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);
    }

    void update(float deltaTime) {
        if (!active) return;
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance < speed * deltaTime) {
            position = target;
            active = false; // Bullet reached target
        } else {
            direction /= distance; // Normalize
            position += direction * speed * deltaTime;
        }
        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window) const {
        if (active) window.draw(shape);
    }

    bool isActive() const { return active; }

private:
    sf::Vector2f position;
    sf::Vector2f target;
    bool active;
    float speed;
    sf::CircleShape shape;
    sf::Clock animationTimer; // Added animationTimer to track elapsed time
};
#endif