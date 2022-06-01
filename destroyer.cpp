#include "destroyer.h"
#include <cmath>

// Constructor
Destroyer::Destroyer(sf::Texture &enemy_texture, sf::Vector2i windowSize)
{
    this->setTexture(enemy_texture);
    this->setScale(0.2, 0.2); // 120x120

    // Set y_start and current_y and Spawn Position
    current_y_ = y_start_; // for first iteration, current_y is where enemy spawns
    this->setSpawnpostion(windowSize.x/2, y_start_ + this->getGlobalBounds().height/2);

    // Using local bounds gives the value wrt to the unscaled texture which is what is required
    // Commenting next line out gives illusion of random movement
    this->setOrigin(this->getLocalBounds().width/2, this->getLocalBounds().height/2);
}

// Enemy Destroyer movement
void Destroyer::animateDestroyer(sf::Time elapsed)
{
    // a * sin(bx + c). a is amplitude. b is frequency. c is offset.
    this->move(20 * std::sin(0.1 * this->getPosition().y - 0), speedy_ * elapsed.asSeconds());
    this->rotate(this->rotation_angle * elapsed.asSeconds());

    if(this->getOrigin() != sf::Vector2f(0, 0))
    {
        current_y_ = this->getPosition().y - this->getGlobalBounds().height/2; // subtract half of height to account for the custom origin
    }
    this->isEscape();
}
