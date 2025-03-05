#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Entities/enemy.h"
#include "Entities/player.h"
#include "entitymanager.h"
#include "playerstats.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <vector>
#include <chrono>

int main()
{
    srand(time(NULL));
    
    // Load & apply textures

    sf::Texture xhairTexture;

    if(!xhairTexture.loadFromFile("assets/Textures/xhair.png")){
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

    sf::Vector2u userDesktopSize = sf::VideoMode::getDesktopMode().size;

    unsigned int resolution = ((userDesktopSize.x > userDesktopSize.y) ? userDesktopSize.y : userDesktopSize.x) * .75;

    sf::RenderWindow window(sf::VideoMode({resolution,resolution}), "Mouse Pointer");
    window.setPosition({static_cast<int>((userDesktopSize.x/2)-(resolution/2)), static_cast<int>((userDesktopSize.y/2)-(resolution/2))});
    window.setMouseCursorVisible(false);

    // Make score & accuracy counter

    PlayerStats playerStats = PlayerStats(font, static_cast<sf::Vector2f>(window.getSize()));

    // Make entity manager
    EntityManager em = EntityManager("assets/Textures/enemy.png", "assets/Sounds/enemydied.wav", playerStats);
    em.setEnemySpawnBoundaries(10, resolution-150, 100, resolution-200);

    // Make Triangle

    Player pointer = Player(playerStats, em, {10, resolution-150, 100, resolution-200});

    // Make enemy
    em.spawnEnemy(100, "assets/Sounds/hit.wav");

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

                pointer.adjust({10, static_cast<unsigned int>(nSize.x-150), 100, static_cast<unsigned int>(nSize.y-200)});
                playerStats.adjustToWindowSize(nSize);
                em.setEnemySpawnBoundaries(10, nSize.x-150, 100, nSize.y-200);
                
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
                    em.spawnEnemy(100, "assets/Sounds/hit.wav");
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