/*
 * Space Invaders
 *     by ProjectX
 *
 * Authors:
 *  George
 *  Ashish
 *  Robel
 *
*/

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <SFML/Audio.hpp>
#include <fstream>
#include <memory>

#include <player.h>
#include <enemy.h>
#include <bullet.h>
#include <animation.h>
#include <ship.h>
#include <destroyer.h>

// Setting class
class Setting : public sf::Sprite
{
private:
    bool check;

public:
    sf::Text text;

public:
    // Constructor
    Setting(sf::Texture &texture, bool checkval)
    {
        this->setTexture(texture);
        this->setScale(0.75, 0.75);
        check = checkval;
        setTextureBox();
    }
    // Getter for check
    bool getCheck()
    {
        return check;
    }
    // Setter for check
    void setCheck(bool checkval)
    {
        check = checkval;
        setTextureBox();
    }

private:
    // Set texture rect
    void setTextureBox()
    {
        if(check) this->setTextureRect(sf::IntRect(0, 0, 68, 64));
        else this->setTextureRect(sf::IntRect(0, 68, 68, 64));
    }
};

void makeEnemyShip(sf::Texture &enemy_texture, sf::Vector2i windowSize, int enemy_speed,
                   Player &player, std::vector<std::unique_ptr<Enemy>> &enemies)
{
//    Ship ship(enemy_texture, windowSize);
    std::unique_ptr<Enemy> enemy = std::make_unique<Ship>(enemy_texture, windowSize);
//    auto enemy = std::make_unique<Ship>(enemy_texture, windowSize); // Alternative

    // Dynamic cast
    Ship *ship = dynamic_cast<Ship*>(enemy.get());

    // Set speed and random direction
    bool direction = std::rand() % 2;
    if(direction) ship->setSpeed(enemy_speed, 0);
    else ship->setSpeed(-enemy_speed, 0);

    ship->setyLimit(player.getPlayerPosition().y);
    ship->setVshift(52); // Change value to change difficulty // Ideal Values(factors of 416 - 16, 26, 32, 52, 104)

    enemies.emplace_back(std::move(enemy)); // std::move to pass ownership to vector
}

void makeEnemyDestroyer(sf::Texture &enemy_texture, sf::Vector2i windowSize, int speed,
                        Player &player, std::vector<std::unique_ptr<Enemy>> &enemies)
{
    auto destroyer = std::make_unique<Destroyer>(enemy_texture, windowSize); // Alternative
    destroyer->setSpeed(0, speed);
    destroyer->rotation_angle = 100;

    destroyer->setyLimit(player.getPlayerPosition().y);
    enemies.emplace_back(std::move(destroyer));
}

void makeText(sf::Text &text, sf::Font &font, sf::String string, sf::Uint32 style, sf::Color fillcolor, sf::Color outlinecolor,
              float outlinethickness, int charsize)
{
    text.setFont(font);
    text.setString(string);
    text.setStyle(style);
    text.setFillColor(fillcolor);
    text.setOutlineColor(outlinecolor);
    text.setOutlineThickness(outlinethickness);
    text.setCharacterSize(charsize);
}

void write_highscore(int highscore)
{
    std::ofstream ofile("highscore.txt");
    if (!ofile.is_open()) std::cout << "File was not opened!" << std::endl;
    ofile << highscore;
    ofile.close(); // Close file
}

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
    makeText(fps_text, myfont, std::to_string(window_fps), sf::Text::Regular, sf::Color::Green, sf::Color::Black, 0, 20);
    fps_text.setPosition(windowSize.x - fps_text.getGlobalBounds().width -5, 0); // -5 for better visibility

    // Set Background
    sf::Texture space_texture;
    if(!space_texture.loadFromFile("Textures/background.jpg")) std::cerr << "Could not load space texture." << std::endl;
    sf::Sprite space_background;
    space_background.setTexture(space_texture);
    space_background.setPosition(0, 0);

    // ------Score Text----
    int score = 0;
    sf::Text score_text;
    makeText(score_text, myfont, "Score: " + std::to_string(score), sf::Text::Bold, sf::Color::Green, sf::Color::Black, 1, 25);
    score_text.setPosition(5, 0); // +5 for better visibility

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
    pause_button.setPosition(windowSize.x/2 -pause_button.getGlobalBounds().width -5, 0 +2); // +2 and -5 for better visibility

    // Control Keys - [<- ->] or [A D]
    bool arrow_control = true;
    sf::Keyboard::Key left_key;
    sf::Keyboard::Key right_key;

    // Player/Spaceship
    sf::Texture player_texture;
    if(!player_texture.loadFromFile("Textures/player.png")) std::cerr << "Could not load spaceship/player texture." << std::endl;
    Player player(player_texture, windowSize);
    player.setSpeed(500);

    // Random Seed
    std::srand(std::time(NULL)); // For random spawn location of enemies

    // Enemy
    std::vector<std::unique_ptr<Enemy>> enemies;
    // Ship Enemy
    sf::Texture enemy_texture;
    if(!enemy_texture.loadFromFile("Textures/enemy.png")) std::cerr << "Could not load enemy texture." << std::endl;
    int enemyShip_count = 8;
    int enemyShip_speed = 250; // Set enemy speed
    for(int i = 0; i < enemyShip_count; i++) // Initialize with fixed number of enemies
    {
        makeEnemyShip(enemy_texture, windowSize, enemyShip_speed, player, enemies);
    }
    // Destroyer Enemy
    sf::Texture destroyer_texture;
    if(!destroyer_texture.loadFromFile("Textures/destroyer.png")) std::cerr << "Could not load destroyer texture." << std::endl;
    int score_spawn = 0; // Score value for when destroyer was spawned

    // Bullet
    sf::Texture bullet_texture;
    if(!bullet_texture.loadFromFile("Textures/bullet.png")) std::cerr << "Could not load bullet texture." << std::endl;
    Bullet bullet(bullet_texture, player.getPlayerPosition());
    bullet.setSpeed(500); // Set bullet speed

    // Explosion Animation
    // Load Textures
    sf::Texture explosion1_texture;
    if(!explosion1_texture.loadFromFile("Textures/explosion1.png")) std::cerr << "Could not load explosion1 texture." << std::endl;
    sf::Texture explosion2_texture;
    if(!explosion2_texture.loadFromFile("Textures/explosion2.png")) std::cerr << "Could not load explosion2 texture." << std::endl;
    std::vector<Animation> animations;

    // Game Over
    bool gameover = false;
    sf::Texture gameover_texture;
    if(!gameover_texture.loadFromFile("Textures/gameover.png")) std::cerr << "Could not load gameover texture." << std::endl;
    sf::Sprite gameover_background;
    gameover_background.setTexture(gameover_texture);
    gameover_background.setPosition(0, 0);

    // Play Again
    sf::Text playagain_button;
    makeText(playagain_button, myfont, "PLAY AGAIN", sf::Text::Bold | sf::Text::Underlined, sf::Color::Red, sf::Color::Black, 1.5, 50);
    playagain_button.setPosition(windowSize.x/2 - playagain_button.getGlobalBounds().width/2, 500);

    // Start Menu
    sf::Texture title_texture;
    if(!title_texture.loadFromFile("Textures/title.png")) std::cerr << "Could not load title texture." << std::endl;
    sf::Sprite title;
    title.setTexture(title_texture);
    title.setPosition(0, 0);
    bool startmenu = true;
    // Start Button
    sf::Text start_button;
    makeText(start_button, myfont, "START", sf::Text::Bold | sf::Text::Underlined, sf::Color::Yellow, sf::Color::Red, 2, 50);
    start_button.setPosition(windowSize.x/4 - start_button.getGlobalBounds().width/2, 450);

    sf::Text start_help;
    makeText(start_help, myfont, "[Press ENTER]", sf::Text::Bold, sf::Color::Yellow, sf::Color::Red, 2, 25);
    start_help.setPosition(windowSize.x/4 - start_help.getGlobalBounds().width/2, 520);

    // Quit Button
    sf::Text quit_button;
    makeText(quit_button, myfont, "QUIT", sf::Text::Bold | sf::Text::Underlined, sf::Color::Yellow, sf::Color::Red, 2, 50);
    quit_button.setPosition(windowSize.x * (0.75) - quit_button.getGlobalBounds().width/2, 450);
    sf::Text quit_help;
    makeText(quit_help, myfont, "[Press Q]", sf::Text::Bold, sf::Color::Yellow, sf::Color::Red, 2, 25);
    quit_help.setPosition(windowSize.x * (0.75) - quit_help.getGlobalBounds().width/2, 520);

    // Exit Button (With sprite)
    sf::Texture exit_texture;
    if(!exit_texture.loadFromFile("Textures/Buttons/exit.png")) std::cerr << "Could not load exit texture." << std::endl;
    sf::Sprite exit_button;
    exit_button.setTexture(exit_texture);
    exit_button.setScale(1, 1);
    exit_button.setPosition(windowSize.x - exit_button.getGlobalBounds().width, windowSize.y - exit_button.getGlobalBounds().height);

    // Settings Menu
    bool settings = false;
    sf::Texture settings_texture;
    if(!settings_texture.loadFromFile("Textures/Buttons/settings.png")) std::cerr << "Could not load settings texture." << std::endl;
    sf::Sprite settings_button;
    settings_button.setTexture(settings_texture);
    settings_button.setScale(0.78125, 0.78125); // 0.78125 * 64 = 50
    settings_button.setPosition(windowSize.x/2 +5, 0 +2); // +2 and +5 for better visibility
    // Check box
    sf::Texture checkbox_texture;
    if(!checkbox_texture.loadFromFile("Textures/Buttons/checkbox.png")) std::cerr << "Could not load checkbox texture." << std::endl;
    // FPS Setting
    Setting fps_setting(checkbox_texture, true);
    makeText(fps_setting.text, myfont, "Show FPS", sf::Text::Bold, sf::Color::Red, sf::Color::Black, 2, 30);
    fps_setting.text.setPosition(windowSize.x * (0.25) - fps_setting.text.getGlobalBounds().width/2, 150);
    fps_setting.setPosition(windowSize.x/2 -75, fps_setting.text.getPosition().y);
    // Sound Setting
    Setting sound_setting(checkbox_texture, true);
    makeText(sound_setting.text, myfont, "Sound On", sf::Text::Bold, sf::Color::Red, sf::Color::Black, 2, 30);
    sound_setting.text.setPosition(windowSize.x * (0.25) - sound_setting.text.getGlobalBounds().width/2, 250);
    sound_setting.setPosition(windowSize.x/2 -75, sound_setting.text.getPosition().y);
    // Music Setting
    Setting music_setting(checkbox_texture, true);
    makeText(music_setting.text, myfont, "Music On", sf::Text::Bold, sf::Color::Red, sf::Color::Black, 2, 30);
    music_setting.text.setPosition(windowSize.x * (0.25) - music_setting.text.getGlobalBounds().width/2, 350);
    music_setting.setPosition(windowSize.x/2 -75, music_setting.text.getPosition().y);
    // Side Teleport Setting
    Setting teleport_setting(checkbox_texture, false);
    makeText(teleport_setting.text, myfont, "Side Teleport", sf::Text::Bold, sf::Color::Red, sf::Color::Black, 2, 30);
    teleport_setting.text.setPosition(windowSize.x * (0.25) - teleport_setting.text.getGlobalBounds().width/2, 450);
    teleport_setting.setPosition(windowSize.x/2 -75, teleport_setting.text.getPosition().y);
    // Help Box
    sf::Texture help_t;
    if(!help_t.loadFromFile("Textures/help.png")) std::cerr << "Could not load help texture." << std::endl;
    sf::Sprite helpbox;
    helpbox.setTexture(help_t);
    helpbox.setScale(0.55, 0.55);
    helpbox.setPosition(windowSize.x/2 , 60);

    // Sounds
    sf::SoundBuffer laser_buffer;
    if (!laser_buffer.loadFromFile("Textures/Sounds/laser.wav")) std::cerr << "Could not load laser buffer." << std::endl;
    sf::Sound laser_sound;
    laser_sound.setBuffer(laser_buffer);
    laser_sound.setVolume(25.0f);
    sf::SoundBuffer explosion_buffer;
    if (!explosion_buffer.loadFromFile("Textures/Sounds/explosion.wav")) std::cerr << "Could not load explosion buffer." << std::endl;
    sf::Sound explosion_sound;
    explosion_sound.setBuffer(explosion_buffer);
    explosion_sound.setVolume(25.0f);
    sf::Music music;
    if (!music.openFromFile("Textures/Sounds/music.ogg")) std::cerr << "Could not load music buffer." << std::endl;
    music.setLoop(true);
    music.setVolume(15.0f);
    music.play();
    sf::SoundBuffer levelup_buffer;
    if (!levelup_buffer.loadFromFile("Textures/Sounds/levelup.wav")) std::cerr << "Could not load levelup buffer." << std::endl;
    sf::Sound levelup_sound;
    levelup_sound.setBuffer(levelup_buffer);
    levelup_sound.setVolume(25.0f);
    sf::SoundBuffer gameover_buffer;
    if (!gameover_buffer.loadFromFile("Textures/Sounds/gameover.wav")) std::cerr << "Could not load gameover buffer." << std::endl;
    sf::Sound gameover_sound;
    gameover_sound.setBuffer(gameover_buffer);
    gameover_sound.setVolume(25.0f);

    // Difficulty Increment
    int maxlevel = 10; // Total 10 levels
    int currentlevel = 1; // Increase level every 500 points

    // Side Teleport - Continuous movement of player through sides of window
    bool side_teleport = false;

    // High Score File and Text
    std::ifstream ifile("highscore.txt");
    if (!ifile.is_open()) std::cout << "File was not opened!" << std::endl;
    int highscore = 0;
    ifile >> highscore;
    ifile.close(); // Close file
    sf::Text highscore_text;
    makeText(highscore_text, myfont, "Highscore: " + std::to_string(highscore), sf::Text::Bold, sf::Color::Red, sf::Color::Black, 1, 30);
    highscore_text.setPosition(windowSize.x/2 - highscore_text.getGlobalBounds().width/2, 50);

    // Clock
    sf::Clock clock;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // -----Delta Time----
        sf::Time elapsed = clock.restart();

        // Framerate
        window_fps = 1 / elapsed.asSeconds(); // To check framerate

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

        if(!pause_on && !gameover && !startmenu && !settings) // If game not paused and not over and not in start menu and not in settings menu
        {
            // ------------Player Movement-----------
            if(sf::Keyboard::isKeyPressed(left_key))
            {
                player.movePlayer(elapsed, left_key, windowSize, side_teleport); // Call move function in Player class
            }
            if(sf::Keyboard::isKeyPressed(right_key))
            {
                player.movePlayer(elapsed, right_key, windowSize, side_teleport); // Call move function in Player class
            }

            // -----------Enemy Movement--------------
            for(int i = 0; i < (int)enemies.size(); i++)
            {
                if(enemies[i]->escape)// If escaped then ------------Game Over------------------
                {
                    gameover = true;
                    if(sound_setting.getCheck()) gameover_sound.play();
                    // Check if new highscore
                    if(score > highscore)
                    {
                        highscore = score;
                        highscore_text.setString("Highscore: " + std::to_string(highscore));
                    }
                }
                // Dynamic cast to check which class the enemy is
                Ship *ship = dynamic_cast<Ship*>(enemies[i].get());
                Destroyer *destroyer = dynamic_cast<Destroyer*>(enemies[i].get());
                if(!enemies[i]->alive) // Check if enemy is still shootable/alive. Enemy is not alive
                {
                    enemies.erase(enemies.begin() + i);
                }
                else // Animate alive enemies
                {
                    if(ship != nullptr) // If enemy is ship
                    {
                        ship->animateShip(elapsed, windowSize);
                    }
                    else if(destroyer != nullptr) // If enemy is destroyer
                    {
                        bullet.setSpeed(800); // Increase speed
                        destroyer->animateDestroyer(elapsed);
                    }
                    else
                    {
                        std::cout << "IMPOSSIBLE!!!\n";
                    }
                }
            }
            // Enemy respawn
            if((int)enemies.size() != enemyShip_count)
            {
                for(int i = 0; i < enemyShip_count - (int)enemies.size(); i++)
                {
                    makeEnemyShip(enemy_texture, windowSize, enemyShip_speed, player, enemies);
                }
            }
            if(score % 100 == 0 && score != score_spawn)
            {
                score_spawn = score;
                makeEnemyDestroyer(destroyer_texture, windowSize, 40, player, enemies); // speed = 40
            }

            // -------------Bullet Movement-------------
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && bullet.fired == false)
            {
                bullet.fired = true; // Bullet has been fired
                if(sound_setting.getCheck()) laser_sound.play();
            }
            // ----------Collision Detection-------------
            for(int i = 0; i < (int)enemies.size(); i++)
            {
                sf::FloatRect bulletbounds = bullet.getGbounds();

                Ship *ship = dynamic_cast<Ship*>(enemies[i].get());
                Destroyer *destroyer = dynamic_cast<Destroyer*>(enemies[i].get());
                if(ship != nullptr) // If enemy is ship
                {
                    if(ship->isCollided(bulletbounds))
                    {
                        ship->alive = false;
//                        bullet.alive = false; // in case of rapid bullet implementation
                        bullet.fired = false;
                        score += 10; // 10 points for each collision
                        // Create animation
                        Animation ship_explosion(explosion1_texture, 1);
                        ship_explosion.enemyposition = ship->getPosition();
                        ship_explosion.explosion = true;
                        animations.emplace_back(ship_explosion);
                        if(sound_setting.getCheck()) explosion_sound.play();
                    }
                }
                else if(destroyer != nullptr) // If enemy is destroyer
                {
                    if(destroyer->isCollided(bulletbounds))
                    {
                        destroyer->damage += 1;
                        bullet.fired = false;
                        if(sound_setting.getCheck()) explosion_sound.play();
                    }
                    if(destroyer->damage == 3)
                    {
                        bullet.setSpeed(500); // Set back to default

                        destroyer->alive = false;
//                        bullet.alive = false; // in case of rapid bullet implementation
                        bullet.fired = false;
                        score += 10; // 10 points for each collision
                        // Create animation
                        Animation destroyer_explosion(explosion2_texture, 2);
                        // Center explosion wrt to custom destroyer origin if it was set
                        if(destroyer->getOrigin() != sf::Vector2f(0, 0))
                        {
                            destroyer_explosion.enemyposition = sf::Vector2f(destroyer->getPosition().x-destroyer->getGlobalBounds().width/2,
                                                                             destroyer->getPosition().y-destroyer->getGlobalBounds().height/2);
                        }
                        else
                        {
//                            destroyer_explosion.setRotation(destroyer_explosion.getRotation()); // Doesn't work well
                            destroyer_explosion.enemyposition = sf::Vector2f(destroyer->getPosition().x, destroyer->getPosition().y);
                        }
                        destroyer_explosion.explosion = true;
                        animations.emplace_back(destroyer_explosion);
                        if(sound_setting.getCheck()) explosion_sound.play();
                    }
                }
                else
                {
                    std::cout << "IMPOSSIBLE!!!\n";
                }
            }
            bullet.animate(elapsed, player.getPlayerPosition()); // Animate bullet after checking for collision. Otherwise bullet goes through enemy

            // ------------Difficulty Increment-----------
            if(score/500 == currentlevel+1 && currentlevel < maxlevel) // If score has reached points required for next level
            {
                currentlevel++;
                enemyShip_count += 2;
                enemyShip_speed += 25;
                if(sound_setting.getCheck()) levelup_sound.play();
            }

            // ----------Explosion Animation-------------
            for(auto &animation : animations)
            {
                if(animation.explosion) // If explosion == true
                {
                    animation.playAnimation(elapsed);
                }
            }
        }

        // ------------Play Again---------------
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) and gameover)
        {
            gameover = false;
            score = 0;
            currentlevel = 1; // Reset difficulty level
            // Reset to default values
            enemyShip_count = 8;
            enemyShip_speed = 250;
            enemies.clear(); // Clear all enemies
            bullet.fired = false; // Reset bullet
            for(int i = 0; i < enemyShip_count; i++) // Initialize with fixed number of enemies
            {
                makeEnemyShip(enemy_texture, windowSize, enemyShip_speed, player, enemies);
            }
        }

        // ------------Quit - Press Q------------
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            write_highscore(highscore);
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

            if(!gameover && !startmenu) // Only if game not over
            {
                // ----------Pause/Play - Press P and release---------
                if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P && !settings)
                {
                    pause_on = !pause_on; // Beautiful
                }
                // ----------Settings Menu---------
                if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S && !pause_on)
                {
                    settings = !settings;
                }
                // --------Pause/Play - Click Pause/Play Button--------Exit Button----Settings Menu-----------
                if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) // If left mouse button is clicked
                {
                    sf::Vector2i mouseclick_pos = sf::Mouse::getPosition(window);
                    if(!pause_on && !settings) // If game not paused and not in settings menu
                    {
                        if(pause_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // If mouseclick is on pause button
                        {
                            pause_on = true;
                            settings = false; // Settings menu and pause menu should be mutually exclusive
                        }
                        // ----------Settings Button---------------
                        else if(settings_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // If settings button is clicked
                        {
                            settings = true;
                        }
                    }
                    else if(settings) // -----When Settings Menu is Open
                    {
                        if(exit_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // if mouselick is on exit button
                        {
                            write_highscore(highscore);
                            return 0;
                        }
                        else if(settings_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // If settings button is clicked
                        {
                            settings = false;
                        }
                        else if(fps_setting.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // if fps setting is clicked
                        {
                            fps_setting.setCheck(!fps_setting.getCheck());
                        }
                        else if(sound_setting.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // if sound settings is clicked
                        {
                            sound_setting.setCheck(!sound_setting.getCheck());
                        }
                        else if(music_setting.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // if music settings is clicked
                        {
                            if(music_setting.getCheck())
                            {
                                music_setting.setCheck(false);
                                music.stop();
                            }
                            else if(!music_setting.getCheck())
                            {
                                music_setting.setCheck(true);
                                music.play();
                            }
                        }
                        else if(teleport_setting.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // if teleport settings is clicked
                        {
                            side_teleport = !side_teleport;
                            teleport_setting.setCheck(side_teleport);
                        }
                    }
                    else if(pause_on)// If game is paused
                    {
                        if(play_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // If mouseclick is on play button
                        {
                            pause_on = false;
                        }
                        else if(exit_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // if mouselick is on exit button
                        {
                            write_highscore(highscore);
                            return 0;
                        }
                    }
                }
                // ------------Side Teleport - Press T------------------
                if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::T)
                {
                    side_teleport = !side_teleport;
                    teleport_setting.setCheck(side_teleport);
                }
            }
            else if(gameover)
            {
                // ------------Play Again---------------
                if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) // If left mouse button is clicked
                {
                    sf::Vector2i mouseclick_pos = sf::Mouse::getPosition(window);
                    if(playagain_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // If play again button is clicked
                    {
                        gameover = false;
                        score = 0;
                        currentlevel = 1; // Reset difficulty level
                        // Reset to default values
                        enemyShip_count = 8;
                        enemyShip_speed = 250;
                        enemies.clear(); // Clear all enemies
                        bullet.fired = false; // Reset bullet
                        for(int i = 0; i < enemyShip_count; i++) // Initialize with fixed number of enemies
                        {
                            makeEnemyShip(enemy_texture, windowSize, enemyShip_speed, player, enemies);
                        }
                    }
                    else if(exit_button.getGlobalBounds().contains(mouseclick_pos.x, mouseclick_pos.y)) // if mouselick is on exit button
                    {
                        write_highscore(highscore);
                        return 0;
                    }
                }
            }

            // ----------Mute - Press M [For all sounds]------------
            if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::M)
            {
                if(sound_setting.getCheck() != music_setting.getCheck())
                {
                    sound_setting.setCheck(false);
                    music_setting.setCheck(false);
                }
                else
                {
                    sound_setting.setCheck(!sound_setting.getCheck());
                    if(music_setting.getCheck())
                    {
                        music_setting.setCheck(false);
                        music.stop();
                    }
                    else if(!music_setting.getCheck())
                    {
                        music_setting.setCheck(true);
                        music.play();
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

            // -----------Start Game------------------
            if(startmenu)
            {
                if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter) startmenu = false;
                else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                    if(start_button.getGlobalBounds().contains(mousepos.x, mousepos.y)) // if start button is clicked
                    {
                        startmenu = false;
                    }
                    else if(quit_button.getGlobalBounds().contains(mousepos.x, mousepos.y)) // if quit button is clicked
                    {
                        write_highscore(highscore);
                        return 0;
                    }
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything in Correct Order
        window.draw(space_background); // Draw space background first

        if(!startmenu) // If not on start menu
        {
            if(fps_setting.getCheck()) // Draw fps
            {
                fps_text.setString(std::to_string(window_fps));
                fps_text.setPosition(windowSize.x - fps_text.getGlobalBounds().width -5, 0); // Required when fps is more than 2 digits
                window.draw(fps_text);
            }
            bullet.drawBullet(window); // Draw bullet
            player.drawPlayer(window); // Draw player/spacship
            for(auto &enemy : enemies) // Iterate through enemies vector
            {
                window.draw(*enemy); // Draw Enemy
            }
            for(auto &animation : animations)
            {
                if(animation.explosion) window.draw(animation); // Draw/Animate explosion
            }

            if(pause_on) // If game is paused
            {
                window.draw(overlay); // Draw overlay
                window.draw(play_button); // Draw play button
                window.draw(exit_button); // Draw exit button
            }
            else if(gameover) // If game over
            {
                window.draw(overlay); // Draw overlay
                window.draw(gameover_background); // Draw gameover sprite
                window.draw(playagain_button); // Draw play again prompt
                window.draw(exit_button); // Draw exit button
                window.draw(highscore_text); // Draw the high score
            }
            else if(settings) // if settings is true
            {
                window.draw(overlay);
                window.draw(exit_button);
                window.draw(settings_button);
                window.draw(fps_setting.text);
                window.draw(fps_setting);
                window.draw(sound_setting.text);
                window.draw(sound_setting);
                window.draw(music_setting.text);
                window.draw(music_setting);
                window.draw(teleport_setting.text);
                window.draw(teleport_setting);
                window.draw(helpbox);
            }
            else // If game is being played
            {
                window.draw(pause_button);
                window.draw(settings_button); // Draw settings button
            }
            window.draw(score_text); // Draw score
        }
        else // If start menu
        {
            window.draw(title); // Draw the title
            window.draw(start_button); // Draw the start button
            window.draw(start_help);
            window.draw(quit_button); // Draw the quit button
            window.draw(quit_help);
        }

        // end the current frame
        window.display();
    }
    return 0;
}
