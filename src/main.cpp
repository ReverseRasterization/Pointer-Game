#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entities/entity.h"
#include "Entities/enemy.h"
#include "Entities/player.h"
#include "playerstats.h"
#include <iostream>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <chrono>


int main()
{
    srand(time(NULL));

    int ENEMY_HEALTH = 100;
    
    // Load & apply textures

    sf::Texture xhairTexture;
    sf::Texture enemyTexture;

    if(!xhairTexture.loadFromFile("assets/Textures/xhair.png")){
        return -1;
    }
    if (!enemyTexture.loadFromFile("assets/Textures/enemy.png")){
        return -1;
    }

    sf::RectangleShape xhair({75.f, 75.f});
    xhair.setTexture(&xhairTexture);
    xhair.setOrigin({37.5f,37.5f});

    // Load sounds

    sf::SoundBuffer gunshot_buffer;
    sf::SoundBuffer hit_buffer;
    sf::SoundBuffer enemy_death_buffer;
    if (!gunshot_buffer.loadFromFile("assets/Sounds/gunshot.wav") || !hit_buffer.loadFromFile("assets/Sounds/hit.wav") || !enemy_death_buffer.loadFromFile("assets/Sounds/enemydied.wav"))
        return -1;

    sf::Sound gunshot (gunshot_buffer);
    sf::Sound hitSound(hit_buffer);
    sf::Sound eDeathSound(enemy_death_buffer);

    // Load Fonts
    sf::Font font;
    if(!font.openFromFile("assets/font.ttf"))
    {
        return -1;
    }

    // Make window

    sf::RenderWindow window(sf::VideoMode({1000,1000}), "Mouse Pointer");
    window.setMouseCursorVisible(false);

    // Make score & accuracy counter

    PlayerStats playerStats = PlayerStats(font, static_cast<sf::Vector2f>(window.getSize()));

    // Make Triangle

    Player pointer = Player(gunshot, playerStats);

    // Make enemy
    auto enemy = std::make_shared<Enemy>(enemyTexture);
    pointer.registerEnemy(enemy);

    sf::Clock clock;


    while (window.isOpen())
    {
        // Events

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if(event->is<sf::Event::Resized>()) {
                std::cout << "\nNew Window Size: \n   X: " << window.getSize().x << "\n   Y: " << window.getSize().y;

                sf::Vector2f nSize = static_cast<sf::Vector2f>(window.getSize());

                pointer.adjust(nSize);
                playerStats.adjustToWindowSize(nSize);
                
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, nSize)));
            }

            if (event->is<sf::Event::MouseMoved>()){
                sf::Vector2f MousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                pointer.pointTo(MousePos);
                xhair.setPosition(MousePos);
            }

            if(event->is<sf::Event::MouseButtonPressed>()){
                pointer.fireBullet(sf::Mouse::getPosition(window));
                
            }

            if(event->is<sf::Event::KeyPressed>()){
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
                    //enemy = Enemy(ENEMY_HEALTH, enemyTexture); TODO: FIX
                }
            }
        }

        // Delta time calculation

        sf::Time dTime = clock.restart();
        float dt = dTime.asSeconds();

        // Rendering

        window.clear();

        // Render all bullets & check if it hit an enemy

        // for (auto it = active_bullets.begin(); it != active_bullets.end();){
            
        //     float newX = it->bullet.getPosition().x + (it->direction.x * it->speed * dt);
        //     float newY = it->bullet.getPosition().y + (it->direction.y * it->speed * dt);

        //     it->bullet.setPosition(sf::Vector2f(newX, newY));

        //     std::vector<int> enemyBounds = enemy.getHitBox();

        //     if (enemy.hit(sf::Vector2f(newX, newY))){
        //         hitsLanded+=1;

        //         float damage = it->base_damage;
        //         float damage_modifier = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f;

        //         if (damage_modifier < .3){
        //             damage_modifier = .3;
        //         }

        //         damage*=damage_modifier;

        //         enemy.takeDamage(static_cast<int>(damage), damage_modifier);

        //         if (damage_modifier >= 1.4) { // Critial hit
        //             score+=4;
        //         }else { // Regular hit
        //             score+=1;
        //         }

        //         if (enemy.getHp() <= 0) {
        //             eDeathSound.play();
        //             enemy = Enemy(ENEMY_HEALTH, enemyTexture);

        //             score+=15;
        //         }else {
        //             hitSound.play();
        //         }

        //         updateScore(scoreLabel, static_cast<sf::Vector2f>(window.getSize()), score);
        //         updateAccuracy(accuracyLabel, bulletsFired, hitsLanded);
                
        //         it = active_bullets.erase(it);
        //         break;
        //     }

        //     if (newX < 0 || newX > window.getSize().x || newY < 0 || newY > window.getSize().y){
        //         it = active_bullets.erase(it);
        //         updateAccuracy(accuracyLabel, bulletsFired, hitsLanded);
        //         break;
        //     }else {
        //         window.draw(it->bullet);
        //         ++it;
        //     }
        // };

        enemy->draw(window);
        pointer.draw(window, dt);
        window.draw(xhair);
        playerStats.draw(window);
        window.display();
    }


    return 0;
}