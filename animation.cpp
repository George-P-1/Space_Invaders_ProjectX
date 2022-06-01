#include "animation.h"

#include <iostream>

Animation::Animation(sf::Texture &texture, int t)
{
    this->setTexture(texture);
    this->setTextureRect(sf::IntRect(0, 0, spritedim, spritedim));
    this->type  = t;
    if(this->type == 1) setShipParameters();
    else if(this->type == 2) setDestroyerParameters();
}

void Animation::playAnimation(sf::Time &elapsed)
{
    if(this->type == 1) this->playShipAnimation(elapsed);
    else if(this->type == 2) this->playDestroyerAnimation(elapsed);
}

// For new animation spritesheet
void Animation::setShipParameters()
{
    this->setScale((float)1/2, (float)1/2); // 256 / 2 = 128
    spritedim = 256; // Dimension of one sprite in sprite sheet. 256x256
    switch_time = 0.02; // 0.02 -> 50 fps
    totalrow = 1;
    totalcolumn = 64;
    currentrow = 1;
    currentcolumn = 1;
}

void Animation::playShipAnimation(sf::Time &elapsed)
{
    // Set animation position
    // Center animation wrt to enemy sprite
    //  ______
    // |  __  |      __
    // | |__| | b   |__| a  [ x - (b-a)/2, y - (b-a)/2 ]
    // |______|
    this->setPosition(this->enemyposition.x - (this->getGlobalBounds().width - 64)/2,
                      this->enemyposition.y - (this->getGlobalBounds().height - 64)/2);

    this->total_time += elapsed.asSeconds();

    if(this->total_time >= this->switch_time)
    {
        this->total_time -= this->switch_time; // Almost same as total_time = 0;. In the long run this will prevent errors caused by approximations.
        this->setTextureRect(sf::IntRect((this->currentcolumn-1) * this->spritedim, 0, this->spritedim, this->spritedim));
        this->currentcolumn++; // Go to next column
        if(this->currentcolumn >= this->totalcolumn) // If one animation is done
        {
            this->explosion = false;
            this->currentcolumn = 1;
        }
    }
}

void Animation::setDestroyerParameters()
{
//    this->setScale((float)1/1.6, (float)1/1.6); // 192 / 1.6 = 120
    this->setScale((float)1/1.2, (float)1/1.2); // 192 / 1.2 = 160
    spritedim = 192; // 192x192
    switch_time = 0.02; // 0.02 -> 50 fps
    totalrow = 1;
    totalcolumn = 64;
    currentrow = 1;
    currentcolumn = 1;
}

void Animation::playDestroyerAnimation(sf::Time &elapsed)
{
    // Set animation position
    this->setPosition(this->enemyposition.x, this->enemyposition.y); // Center animation wrt to enemy sprite

    this->total_time += elapsed.asSeconds();

    if(this->total_time >= this->switch_time)
    {
        this->total_time -= this->switch_time; // Almost same as total_time = 0;. In the long run this will prevent errors caused by approximations.
        this->setTextureRect(sf::IntRect((this->currentcolumn-1) * this->spritedim, 0, this->spritedim, this->spritedim));
        this->currentcolumn++; // Go to next column
        if(this->currentcolumn >= this->totalcolumn) // If one animation is done
        {
            this->explosion = false;
            this->currentcolumn = 1;
        }
    }
}
