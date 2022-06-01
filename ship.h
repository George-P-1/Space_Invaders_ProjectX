#pragma once

#include <enemy.h>

class Ship : public Enemy
{
private:
    int vertical_shift_; // Will determine how fast the enemy ship reaches the y_limit_

public:
    // Constructor
    Ship(sf::Texture &enemy_texture, sf::Vector2i windowSize);
    // Setter for vertical shift
    void setVshift(int vshift);
    // Enemy Ship movement
    void animateShip(sf::Time elapsed, sf::Vector2i windowSize);

private:
    // Enemy bounce off sides of window
    void bounce_shift(sf::Vector2i windowSize);
};
