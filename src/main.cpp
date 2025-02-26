#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Entities/enemy.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <chrono>

// TODO: Add a settings UI that adjusts volume, max enemies, enemies per second, toggles the FPS counter or the accuracy counter, and playing field size

struct Bullet{
    sf::Vector2f direction;

    float speed = 2000;
    int base_damage = 15;

    sf::CircleShape bullet = sf::CircleShape(5.0f);
};


Bullet fireBullet(sf::Vector2f mousePos, sf::Vector2f trianglePos);
void updateScore(sf::Text& scoreLabel, sf::Vector2f windowSize, int nScore);
void updateAccuracy(sf::Text& accuracyLabel, int bulletsFired, int hitsLanded);

int main()
{
    srand(time(NULL));

    int ENEMY_HEALTH = 100;

    int score = 0;
    int bulletsFired = 0;
    int hitsLanded = 0;
    
    // Load & apply textures

    sf::Texture xhairTexture;
    sf::Texture enemyTexture;

    if(!xhairTexture.loadFromFile("assets/Textures/xhair.png")){
        return -1;
    }

    sf::RectangleShape xhair({75.f, 75.f});
    xhair.setTexture(&xhairTexture);
    xhair.setOrigin({37.5f,37.5f});

    if (!enemyTexture.loadFromFile("assets/Textures/enemy.png")){
        return -1;
    }

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

    sf::Text scoreLabel(font);
    scoreLabel.setCharacterSize(48);
    scoreLabel.setFillColor(sf::Color::White);
    updateScore(scoreLabel, static_cast<sf::Vector2f>(window.getSize()), score);

    sf::Text accuracyLabel(font);
    accuracyLabel.setCharacterSize(24);
    accuracyLabel.setFillColor(sf::Color::White);
    accuracyLabel.setPosition(sf::Vector2f(10, 970));

    // Make Triangle

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, {0, -25}); // This is the tip
    triangle.setPoint(1, {25, 25});
    triangle.setPoint(2, {-25, 25});
    triangle.setFillColor(sf::Color::Green);
    triangle.setOrigin({0,0});
    triangle.setPosition({500, 500});

    // Make enemy

    Enemy enemy = Enemy(ENEMY_HEALTH, enemyTexture);

    std::vector<Bullet> active_bullets;

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

                triangle.setPosition(nSize/2.f);
                accuracyLabel.setPosition({10.f, nSize.y-30.f});
                updateScore(scoreLabel, nSize, score);
                
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, nSize)));
            }

            if (event->is<sf::Event::MouseMoved>()){
                sf::Vector2f MousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                sf::Vector2f TrianglePos = triangle.getPosition();
                double dX = (double)MousePos.x - TrianglePos.x;
                double dY = (double)MousePos.y - TrianglePos.y;

                triangle.setRotation(sf::degrees((atan2(dY, dX)) * (180.0f / 3.14159265359f)+90));
                xhair.setPosition(MousePos);
            }

            if(event->is<sf::Event::MouseButtonPressed>()){
                active_bullets.push_back(fireBullet(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), triangle.getPosition()));
                gunshot.play();

                bulletsFired+=1;
            }

            if(event->is<sf::Event::KeyPressed>()){
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
                    enemy = Enemy(ENEMY_HEALTH, enemyTexture);
                }
            }
        }

        // Delta time calculation

        sf::Time dTime = clock.restart();
        float dt = dTime.asSeconds();

        // Rendering

        window.clear();

        // Render all bullets & check if it hit an enemy

        for (auto it = active_bullets.begin(); it != active_bullets.end();){
            
            float newX = it->bullet.getPosition().x + (it->direction.x * it->speed * dt);
            float newY = it->bullet.getPosition().y + (it->direction.y * it->speed * dt);

            it->bullet.setPosition(sf::Vector2f(newX, newY));

            std::vector<int> enemyBounds = enemy.getHitBox();

            if (enemy.hit(sf::Vector2f(newX, newY))){
                hitsLanded+=1;

                float damage = it->base_damage;
                float damage_modifier = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f;

                if (damage_modifier < .3){
                    damage_modifier = .3;
                }

                damage*=damage_modifier;

                enemy.takeDamage(static_cast<int>(damage), damage_modifier);

                if (damage_modifier >= 1.4) { // Critial hit
                    score+=4;
                }else { // Regular hit
                    score+=1;
                }

                if (enemy.getHp() <= 0) {
                    eDeathSound.play();
                    enemy = Enemy(ENEMY_HEALTH, enemyTexture);

                    score+=15;
                }else {
                    hitSound.play();
                }

                updateScore(scoreLabel, static_cast<sf::Vector2f>(window.getSize()), score);
                updateAccuracy(accuracyLabel, bulletsFired, hitsLanded);
                
                it = active_bullets.erase(it);
                break;
            }

            if (newX < 0 || newX > window.getSize().x || newY < 0 || newY > window.getSize().y){
                it = active_bullets.erase(it);
                updateAccuracy(accuracyLabel, bulletsFired, hitsLanded);
                break;
            }else {
                window.draw(it->bullet);
                ++it;
            }
        };

        enemy.draw(window);
        window.draw(triangle);
        window.draw(xhair);
        window.draw(scoreLabel);
        window.draw(accuracyLabel);
        window.display();
    }


    return 0;
}

Bullet fireBullet(sf::Vector2f mousePos, sf::Vector2f trianglePos) {
    Bullet nBullet;
    nBullet.direction = sf::Vector2f(mousePos.x - trianglePos.x, mousePos.y - trianglePos.y);
    nBullet.bullet.setPosition(sf::Vector2f(trianglePos.x-5, trianglePos.y-5));

    float magnitude = std::sqrt(nBullet.direction.x * nBullet.direction.x + nBullet.direction.y * nBullet.direction.y);
    if (magnitude != 0){
        nBullet.direction.x /= magnitude;
        nBullet.direction.y /= magnitude;
    }

    return nBullet;
}

void updateScore(sf::Text& scoreLabel, sf::Vector2f windowSize, int nScore){
    scoreLabel.setString("Score: " + std::to_string(nScore));

    scoreLabel.setOrigin(scoreLabel.getLocalBounds().getCenter());
    scoreLabel.setPosition({windowSize.x/2.f,30});
}

void updateAccuracy(sf::Text& accuracyLabel, int bulletsFired, int hitsLanded) {
    if (bulletsFired != 0 && hitsLanded != 0) {
        accuracyLabel.setString("Accuracy: " + std::to_string(static_cast<int>((static_cast<float>(hitsLanded)/static_cast<float>(bulletsFired))*100)) + '%');
    }
}
