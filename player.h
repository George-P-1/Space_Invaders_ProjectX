#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Player : private sf::Sprite
{
private:
    int speedx_; // Only horizontal speed/movement
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
    void movePlayer(sf::Time elapsed, sf::Keyboard::Key key, sf::Vector2i windowSize);

private:
    // Move Player to exact bounds when outside Window
    void exact_bounds(sf::FloatRect player_bounds, sf::Vector2i windowSize);
};
