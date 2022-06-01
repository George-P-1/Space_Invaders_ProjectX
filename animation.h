#pragma once

#include <SFML/Graphics.hpp>

class Animation : public sf::Sprite
{
public:
    sf::Vector2f enemyposition;
    bool explosion = false;
    float total_time = 0;
    int type = 0; // Animation texture

    // Different values for different animations
    int spritedim;
    float switch_time;
    int totalrow;
    int totalcolumn;
    int currentrow;
    int currentcolumn;


public:
    Animation(sf::Texture &texture, int t);

    void playAnimation(sf::Time &elapsed);

private:
    void setShipParameters();

    void setDestroyerParameters();

    void playShipAnimation(sf::Time &elapsed);

    void playDestroyerAnimation(sf::Time &elapsed);
};
