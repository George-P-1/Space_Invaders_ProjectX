#include "bullet.h"

// Constructor
Bullet::Bullet(sf::Texture &bullet_texture, sf::Vector2f playerposition)
{
    this->setTexture(bullet_texture);
    this->setScale(0.625, 0.625); // 32 * 0.625 = 20

    // Set position
    this->setBulletPosition(playerposition);
}

// Set position for bullet
void Bullet::setBulletPosition(sf::Vector2f playerposition)
{
    this->setPosition(playerposition.x +64/2 -this->getGlobalBounds().width/2, playerposition.y +5);
    // 64/2 to center it wrt spaceship. +5 to hide bullet properly
}

// Setter for speed
void Bullet::setSpeed(int speedy)
{
    speedy_ = speedy;
}

// Draw on window
void Bullet::drawBullet(sf::RenderWindow &window)
{
    window.draw(*this);
}

// Bullet movement
void Bullet::animate(sf::Time elapsed, sf::Vector2f playerposition)
{
    this->isAlive();
    if(!this->fired) // If bullet is not yet fired
    {
        this->setBulletPosition(playerposition);
    }
    else if(this->fired) // If bullet is fired
    {
        this->move(0, -speedy_ * elapsed.asSeconds());
    }
}

// Check if bullet is still in bounds and not collided
void Bullet::isAlive()
{
    // Check if it is bounds of window i.e. not exited the top of the window
    if(this->getGlobalBounds().top < 0.0)
    {
//        this->alive = false; // in case rapid bullets are implemented
        this->fired = false; // So that bullet can be fired again
    }
}

// Get bullet globalbounds to check for collision
sf::FloatRect Bullet::getGbounds()
{
    return this->getGlobalBounds();
}
