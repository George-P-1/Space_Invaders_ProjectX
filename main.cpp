#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

#include <player.h>
#include <enemy.h>

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Space Invaders");
    sf::Vector2i windowSize(window.getSize().x, window.getSize().y);
    // Set Icon
    sf::Image icon;
    if (!icon.loadFromFile("Textures/icon.png")) std::cerr << "Could not load icon." << std::endl;
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Load Font
    sf::Font myfont;
    if(!myfont.loadFromFile("Font/arial.ttf")) std::cerr << "Could not load font." << std::endl;

    // Show FPS
    int window_fps = 60;
    window.setFramerateLimit(window_fps);
    sf::Text fps_text;
    fps_text.setString(std::to_string(window_fps));
    fps_text.setFillColor(sf::Color::Green);
    fps_text.setFont(myfont);
    fps_text.setCharacterSize(20);
    fps_text.move(windowSize.x - fps_text.getGlobalBounds().width -5, 0); // -5 for better visibility

    // Set Background
    sf::Texture space_texture;
    if(!space_texture.loadFromFile("Textures/background.jpg")) std::cerr << "Could not load space texture." << std::endl;
    sf::Sprite space_background;
    space_background.setTexture(space_texture);
    space_background.setPosition(0, 0);

    // ------Score Text----
    int score = 0;
    sf::Text score_text;
    score_text.setString("Score: " + std::to_string(score));
    score_text.setStyle(sf::Text::Bold);
    score_text.setFillColor(sf::Color::Green);
    score_text.setOutlineColor(sf::Color::Black);
    score_text.setOutlineThickness(1);
    score_text.setFont(myfont);
    score_text.setCharacterSize(25);
    score_text.move(0 +5, 0); // +5 for better visibility

    // Grey Overlay for Pause/Play Feature
    sf::Texture overlay_texture;
    if(!overlay_texture.loadFromFile("Textures/gray_overlay.jpg")) std::cerr << "Could not load overlay texture." << std::endl;
    sf::Sprite overlay;
    overlay.setTexture(overlay_texture);
    overlay.setPosition(0, 0);
    overlay.setColor(sf::Color(overlay.getColor().r, overlay.getColor().g, overlay.getColor().b, 200));

    // Play/Pause Feature
    bool pause_on = false;
    sf::Texture play_texture;
    if(!play_texture.loadFromFile("Textures/Buttons/play.png")) std::cerr << "Could not load play button texture." << std::endl;
    play_texture.setSmooth(true); // Smooth borders
    sf::Sprite play_button;
    play_button.setTexture(play_texture);
    play_button.setScale(0.25, 0.25); // 512 * 0.25 = 128
    play_button.setPosition(windowSize.x/2 -play_button.getGlobalBounds().width/2, windowSize.y/2 -play_button.getGlobalBounds().width/2);
    sf::Texture pause_texture;
    if(!pause_texture.loadFromFile("Textures/Buttons/pause.png")) std::cerr << "Could not load pause button texture." << std::endl;
    pause_texture.setSmooth(true); // Smooth borders
    sf::Sprite pause_button;
    pause_button.setTexture(pause_texture);
    pause_button.setScale(0.09765625, 0.09765625);  // 512 * 0.09765625 = 50
    pause_button.setPosition(windowSize.x/2 -pause_button.getGlobalBounds().width/2, 0 +2); // +2 for better visibility

    // Control Keys - [<- ->] or [A D]
    bool arrow_control = true;
    sf::Keyboard::Key left_key;
    sf::Keyboard::Key right_key;

    // Player/Spaceship
    sf::Texture player_texture;
    if(!player_texture.loadFromFile("Textures/player.png")) std::cerr << "Could not load spaceship/player texture." << std::endl;
    Player player(player_texture, windowSize);
    player.setSpeed(400);

    // Random Seed
    std::srand(std::time(NULL)); // For random spawn location of enemies
    // Enemy
    sf::Texture enemy_texture;
    if(!enemy_texture.loadFromFile("Textures/enemy.png")) std::cerr << "Could not load enemy texture." << std::endl;
    // Enemies
    int enemy_count = 8;
    std::vector<Enemy> enemies;
    for(int i = 0; i < enemy_count; i++) // Initialize with fixed number of enemies
    {
        Enemy enemy(enemy_texture, windowSize);
        enemy.setSpeed(300);
        enemy.setyLimit(player.getfixedy());
        enemy.setVshift(52); // Change value to change difficulty // Ideal Values(factors of 416 - 16, 26, 32, 52, 104)
        enemies.emplace_back(enemy);
    }

    // Clock
    sf::Clock clock;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // -----Delta Time----
        sf::Time elapsed = clock.restart();

        // ------Show Score----
        score_text.setString("Score: " + std::to_string(score));

        // -----------Change Control Keys--------
        if(arrow_control) // If controls are set to arrows
        {
            left_key = sf::Keyboard::Left;
            right_key = sf::Keyboard::Right;
        }
        else
        {
            left_key = sf::Keyboard::A;
            right_key = sf::Keyboard::D;
        }
        player.setLeftkey(left_key);
        player.setRightkey(right_key);

        // ------------Player Movement-----------
        if(!pause_on) // If game not paused
        {
            if(sf::Keyboard::isKeyPressed(left_key))
            {
                player.movePlayer(elapsed, left_key, windowSize); // Call move function in Player class
            }
            if(sf::Keyboard::isKeyPressed(right_key))
            {
                player.movePlayer(elapsed, right_key, windowSize); // Call move function in Player class
            }
        }

        // -----------Enemy Movement--------------
        if(!pause_on) // If game not paused
        {
            for(int i = 0; i < (int)enemies.size(); i++)
            {
                if(enemies[i].alive == false) // Check if enemy is still shootable/alive
                {
                    enemies.erase(enemies.begin() + i);
                }
                enemies[i].animate(elapsed, windowSize); // Animate alive enemies
            }
        }

        // ------------Quit - Press Q------------
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            return 0; // return 0 means program worked how it was supposed to.
        }
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // ----------Pause/Play - Press P and release---------
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P)
            {
                pause_on = !pause_on; // Beautiful
            }
            // --------Pause/Play - Click Pause/Play Button--------
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) // If left mouse button is clicked
            {
                sf::Vector2i mouseclick_pos = sf::Mouse::getPosition(window);
                if(!pause_on) // If game not paused
                {
                    if(pause_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // If mouseclick is on pause button
                    {
                        pause_on = true;
                    }
                }
                else // If game is paused
                {
                    if(play_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // If mouseclick is on play button
                    {
                        pause_on = false;
                    }
                }
            }

            // ------Mod for Score-----
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Add) score += 100;

            // ---------Switch Control Keys----------
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::K)
            {
                arrow_control = !arrow_control;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything in Correct Order
        window.draw(space_background); // Draw space background first
        window.draw(fps_text); // Draw fps
        player.drawPlayer(window); // Draw player/spacship
        for(auto &enemy : enemies) // Iterate through enemies vector
        {
            enemy.drawEnemy(window); // Draw Enemy
        }
        if(pause_on == true) // If game is paused
        {
            window.draw(overlay); // Draw overlay
            window.draw(play_button); // Draw play button
        }
        else // If game is being played
        {
            window.draw(pause_button);
        }
        window.draw(score_text); // Draw score

        // end the current frame
        window.display();
    }

    return 0;
}
