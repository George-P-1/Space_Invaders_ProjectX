#include "player.h"

// Constructor
Player::Player(sf::Texture &player_texture, sf::Vector2i windowSize)
{
    this->setTexture(player_texture);
    this->setScale(0.25, 0.25); // 256 * 0.25 = 64

    // Position in center of fixed parallel X-axis.
    this->fixedy_ = windowSize.y -this->getGlobalBounds().height -6; // -6 for better visibility
    this->setPosition(windowSize.x/2 -this->getGlobalBounds().width/2, fixedy_);
}

// Setter for speed
void Player::setSpeed(int speedx)
{
    speedx_ = speedx;
}

// Setters for left and right key
void Player::setLeftkey(sf::Keyboard::Key key)
{
    this->left_key_ = key;
}
void Player::setRightkey(sf::Keyboard::Key key)
{
    this->right_key_ = key;
}

// Draw on window
void Player::drawPlayer(sf::RenderWindow &window)
{
    window.draw(*this);
}

// Move Player to exact bounds when outside Window
void Player::exact_bounds(sf::FloatRect player_bounds, sf::Vector2i windowSize)
{
    if(player_bounds.left < 0.0)
    {
        this->setPosition(0.0, this->getPosition().y);
    }
    else if(player_bounds.left > windowSize.x - player_bounds.width)
    {
        this->setPosition(windowSize.x - player_bounds.width, this->getPosition().y);
    }
}

// Move Player
void Player::movePlayer(sf::Time elapsed, sf::Keyboard::Key key, sf::Vector2i windowSize, bool side_teleport)
{
    sf::FloatRect player_bounds = this->getGlobalBounds();
    if(!side_teleport)
    {
        if(key == this->left_key_ && player_bounds.left > 0.0) // Dont exit screen on left side
        {
            this->move(-speedx_ * elapsed.asSeconds(), 0); // Move towards left
        }
        if(key == this->right_key_ && player_bounds.left < windowSize.x - player_bounds.width) // Dont exit screen on righ side
        {
            this->move(speedx_ * elapsed.asSeconds(), 0); // Move towards right
        }
        this->exact_bounds(player_bounds, windowSize);
    }
    else
    {
        if(key == this->left_key_ && player_bounds.left +player_bounds.width/2 > 0.0) // When left half of spaceship is in the left side of window
        {
            if(player_bounds.contains(0 -player_bounds.width*(0.75) +50, fixedy_)) // +5 for making condition true
            {
                this->setPosition(windowSize.x +this->getGlobalBounds().width/2, fixedy_);
            }
            this->move(-speedx_ * elapsed.asSeconds(), 0); // Move towards left
        }
        if(key == this->right_key_ && player_bounds.left < windowSize.x - player_bounds.width/2) // When right half of spaceship is in right side
        {
            if(player_bounds.contains(windowSize.x +player_bounds.width*(0.75) -50, fixedy_))
            {
                this->setPosition(0 -this->getGlobalBounds().width/2, fixedy_);
            }
            this->move(speedx_ * elapsed.asSeconds(), 0); // Move towards right
        }
    }
}

// Getter for fixedy
sf::Vector2f Player::getPlayerPosition()
{
    return this->getPosition();
}
