#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Player : private sf::Sprite
{
private:
    int speedx_; // Only horizontal speed/movement
    int fixedy_; // Fixed y-coordinate of player/spaceship
    sf::Keyboard::Key left_key_;
    sf::Keyboard::Key right_key_;

public:
    // Constructor
    Player(sf::Texture &player_texture, sf::Vector2i windowSize);
    // Setter for speed
    void setSpeed(int speedx);
    // Setters for left and right key
    void setLeftkey(sf::Keyboard::Key key);
    void setRightkey(sf::Keyboard::Key key);
    // Draw on window
    void drawPlayer(sf::RenderWindow &window);
    // Move Player
    void movePlayer(sf::Time elapsed, sf::Keyboard::Key key, sf::Vector2i windowSize, bool side_teleport);
    // Getter for position
    sf::Vector2f getPlayerPosition();

private:
    // Move Player to exact bounds when outside Window
    void exact_bounds(sf::FloatRect player_bounds, sf::Vector2i windowSize);
};
