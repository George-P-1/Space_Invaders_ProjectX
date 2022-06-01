#pragma once

#include <SFML/Graphics.hpp>

class Enemy : public sf::Sprite
{
public:
    int speedx_; // Horizontal speed
    int speedy_; // Vertical speed
    int y_start_ = 50; // The parallel x-axis(fixed y-coordinate) where enemy gets spawned
    int y_limit_; // The parallel x-axis where the enemy collides with player and game ends
    int current_y_; // Current y-coordinate of enemy
    bool alive = true; // If enemy is still not destroyed
    bool escape = false; // If enemy crossed y-limit

    virtual ~Enemy() = default; // For polymorhpism since its a base class

public:
    // Setter for Horizontal speed
    void setSpeed(int speedx, int speedy);
    // Setter for y_limit
    void setyLimit(int ylimit);
    // Check collision and return bool
    bool isCollided(sf::FloatRect &bulletbounds);
    // Set random spawn position for enemy
    void setSpawnpostion(int x_coordinate, int y_coordinate);
    // Check if enemy crossed the y_limit
    void isEscape();
};
