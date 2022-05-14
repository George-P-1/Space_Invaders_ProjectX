#pragma once

#include <SFML/Graphics.hpp>

class Enemy : private sf::Sprite
{
private:
    int speedx_; // Horizontal speed
    int y_start_; // The parallel x-axis(fixed y-coordinate) where enemy gets spawned
    int y_limit_; // The parallel x-axis where the enemy collides with player and game ends
    int vertical_shift_; // Will determine how fast the enemy reaches the y_limit_
    int current_y_; // Current y-coordinate of enemy

public:
    bool alive = true; // If enemy is still not destroyed

public:
    // Constructor
    Enemy(sf::Texture &enemy_texture, sf::Vector2i windowSize);
    // Setter for Horizontal speed
    void setSpeed(int speedx);
    // Setter for y_limit
    void setyLimit(int ylimit);
    // Setter for vertical shift
    void setVshift(int vshift);
    // Draw on window
    void drawEnemy(sf::RenderWindow &window);
    // Enemy movement
    void animate(sf::Time elapsed, sf::Vector2i windowSize);

private:
    // Set random spawn position for enemy
    void setSpawnpostion(sf::Vector2i windowSize);
    // Enemy bounce off sides of window
    void bounce_shift(sf::Vector2i windowSize);
    // Check if enemy crossed the y_limit
    void isAlive();
};
