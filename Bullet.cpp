#include "Bullet.h"
#include <cmath>
#include <iostream>



// Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos)
//     : position(startPos), target(targetPos), active(true), speed(300.0f) {
//     shape.setRadius(5.0f);
//     shape.setFillColor(sf::Color::Red);
//     shape.setPosition(position);
// }

// void Bullet::update(float deltaTime) {
//     if (!active) return;

//     // Calculate direction toward the target
//     sf::Vector2f direction = target - position;
//     float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

//     if (distance < 5.0f) {
//         // Bullet has reached the target
//         active = false;
//         std::cout << "[Bullet] Hit the target at position (" 
//                   << target.x << ", " << target.y << ")\n";
//         return;
//     }

//     // Normalize direction and move the bullet
//     sf::Vector2f normalizedDirection = direction / distance;
//     position += normalizedDirection * speed * deltaTime;
//     shape.setPosition(position);
// }

// void Bullet::draw(sf::RenderWindow& window) {
//     if (active) {
//         window.draw(shape);
//     }
// }

// bool Bullet::isActive() const {
//     return active;
// }