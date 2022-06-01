#include "enemy.h"

// Set random spawn position for enemy
void Enemy::setSpawnpostion(int x_coordinate, int y_coordinate)
{
    this->setPosition(x_coordinate, y_coordinate);
}

// Setter for Horizontal speed
void Enemy::setSpeed(int speedx, int speedy)
{
    speedx_ = speedx;
    speedy_ = speedy;
}
// Setter for y_limit
void Enemy::setyLimit(int ylimit)
{
    y_limit_ = ylimit;
}

// Check if enemy crossed the y_limit
void Enemy::isEscape()
{
    if(this->current_y_ + this->getGlobalBounds().height > y_limit_)
    {
        this->alive = false;
        this->escape = true;
    }
}

// Check collision and return bool
bool Enemy::isCollided(sf::FloatRect &bulletbounds)
{
    if(this->getGlobalBounds().intersects(bulletbounds))
    {
        return true;
    }
    else return false;
}
