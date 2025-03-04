#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entities/enemy.h"
#include "Entities/player.h"
#include "entitymanager.h"
#include "playerstats.h"
#include <iostream>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <chrono>

void summonEnemy(sf::Texture& enemyTexture, EntityManager& em, int maxEnemies=5) {
    if (em.getEntityCount() == maxEnemies) {return;};

    em.registerEntity(std::make_shared<Enemy>(Enemy(enemyTexture, sf::Vector2i(10, 850), sf::Vector2i(100, 800), "assets/Sounds/enemydied.wav", "assets/Sounds/hit.wav", em)));
}

int main()
{
    srand(time(NULL));
    
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

    // Make entity manager
    EntityManager em = EntityManager();

    // Make Triangle
    Player pointer = Player(playerStats, em);

    // Make enemy
    summonEnemy(enemyTexture, em);

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
                    summonEnemy(enemyTexture, em);
                }
            }
        }

        // Delta time calculation

        sf::Time dTime = clock.restart();
        float dt = dTime.asSeconds();

        // Rendering

        window.clear();

        
        em.drawEntities(window);
        pointer.draw(window, dt);
        window.draw(xhair);
        playerStats.draw(window);
        window.display();
    }


    return 0;
}