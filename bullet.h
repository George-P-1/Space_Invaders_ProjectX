#pragma once

#include <SFML/Graphics.hpp>

class Bullet : private sf::Sprite
{
private:
    int speedy_; // Vertical speed

public:
//    bool alive = true; // If bullet is in screen and has not collided. In case of rapid bullet implementation. when alive bullet may be fired or not
    bool fired = false; // If bullet has been fired or not

public:
    // Constructor
    Bullet(sf::Texture &bullet_texture, sf::Vector2f playerposition);
    // Setter for speed
    void setSpeed(int speedy);
    // Draw on window
    void drawBullet(sf::RenderWindow &window);
    // Bullet movement
    void animate(sf::Time elapsed, sf::Vector2f playerposition);
    // Get bullet globalbounds to check for collision
    sf::FloatRect getGbounds();

private:
    // Set position for bullet
    void setBulletPosition(sf::Vector2f playerposition);
    // Check if bullet is still in bounds and not collided
    void isAlive();
};
