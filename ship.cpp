#include "ship.h"

// Constructor
Ship::Ship(sf::Texture &enemy_texture, sf::Vector2i windowSize)
{
    this->setTexture(enemy_texture);
    this->setScale(0.5, 0.5); // 0.5 * 128 = 64

    // Set y_start and current_y and Spawn Position
    current_y_ = y_start_; // for first iteration, current_y is where enemy spawns
    // Position it randomly in the parallel x-axis
    int x_coordinate = std::rand() % (windowSize.x - (int)this->getGlobalBounds().width); // Random x-coordinate in the parallel x-axis
    this->setSpawnpostion(x_coordinate, y_start_);
}

// Setter for vertical shift
void Ship::setVshift(int vshift)
{
    vertical_shift_ = vshift;
}

// Enemy movement
void Ship::animateShip(sf::Time elapsed, sf::Vector2i windowSize)
{
    this->move(speedx_ * elapsed.asSeconds(), 0);
    this->bounce_shift(windowSize);
    this->isEscape();
}

// Enemy bounce off sides of window
void Ship::bounce_shift(sf::Vector2i windowSize)
{
    sf::FloatRect enemy_bounds = this->getGlobalBounds();
    if(enemy_bounds.left <= 0.0) // Dont exit screen on left side
    {
        this->speedx_ = std::abs(this->speedx_);
        this->current_y_ += this->vertical_shift_; // Add vertical shift value to current y-coordinate of enemy
        this->setPosition(0.0, current_y_); // Set new y-coordinate
    }
    else if(enemy_bounds.left >= windowSize.x - enemy_bounds.width) // Dont exit screen on right side
    {
        this->speedx_ = -std::abs(this->speedx_);
        this->current_y_ += this->vertical_shift_;
        this->setPosition(windowSize.x - enemy_bounds.width, current_y_);
    }
}
