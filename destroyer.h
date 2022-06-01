#pragma once

#include <enemy.h>

class Destroyer : public Enemy
{
public:
    float rotation_angle; // Rotation value
    int damage = 0; // When damage is 3 it should be destroyed

public:
    // Constructor
    Destroyer(sf::Texture &enemy_texture, sf::Vector2i windowSize);
    // Enemy Destroyer movement
    void animateDestroyer(sf::Time elapsed);
};
