
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "player.h"
#include "enemy.h"
#include "../playerstats.h"

Player::Player(sf::Sound& gunshotSound, PlayerStats& player_stats): gunshot(gunshotSound), playerStats(player_stats) {
    pointer.setPointCount(3);
    pointer.setPoint(0, {0, -25}); // This is the tip
    pointer.setPoint(1, {25, 25});
    pointer.setPoint(2, {-25, 25});
    pointer.setFillColor(sf::Color::Green);
    pointer.setOrigin({0,0});
    pointer.setPosition({500, 500});
}

void Player::pointTo(sf::Vector2f point)
{
    sf::Vector2f TrianglePos = pointer.getPosition();
    double dX = (double)point.x - TrianglePos.x;
    double dY = (double)point.y - TrianglePos.y;

    pointer.setRotation(sf::degrees((atan2(dY, dX)) * (180.0f / 3.14159265359f)+90));
}

void Player::adjust(sf::Vector2f nSize){
    pointer.setPosition(nSize/2.f);
}

void Player::draw(sf::RenderWindow& window, float deltaTime){
    // Draw the actual triangle
    window.draw(pointer);

    // Draw all the bullets
    for (auto it = active_bullets.begin(); it != active_bullets.end();){
        // Determine its new position
        std::shared_ptr<Bullet> bulletptr = *it;

        float newX = bulletptr->bullet.getPosition().x + (bulletptr->direction.x * bulletptr->speed * deltaTime);
        float newY = bulletptr->bullet.getPosition().y + (bulletptr->direction.y * bulletptr->speed * deltaTime);

        bulletptr->bullet.setPosition(sf::Vector2f(newX, newY));

        window.draw(bulletptr->bullet);

        // Check if it hit any enemies
        bool hit = false;
        for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy){
            std::shared_ptr<Enemy> enemyPtr = *enemy;
            
            if (enemyPtr->hit(newX, newY)){
                float damage = bulletptr->base_damage;
                float damage_modifier = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f;

                if (damage_modifier < .3){
                    damage_modifier = .3;
                }

                damage*=damage_modifier;

                enemyPtr->takeDamage(static_cast<int>(damage));

                if (damage_modifier >= 1.4) { // Critial hit
                    playerStats.changeScore(4);
                }else { // Regular hit
                    playerStats.changeScore(1);
                }

                it = active_bullets.erase(it);
                hit = true;
                bulletsHit+=1;

                playerStats.setAccuracy(bulletsFired, bulletsHit);
                
                break;
            }
        }

        // Check if the bullet went out of bounds, if so then derender it
        if (!hit) {
            if (newX < 0 || newX > window.getSize().x || newY < 0 || newY > window.getSize().y){
                it = active_bullets.erase(it);
                playerStats.setAccuracy(bulletsFired, bulletsHit);
                break;
            }else {
                window.draw(bulletptr->bullet);
                ++it;
            }
        }
    }
}

void Player::fireBullet(sf::Vector2i target){
    gunshot.play();

    auto nBullet = std::make_shared<Bullet>();
    sf::Vector2f origin = pointer.getPosition();

    nBullet->direction = sf::Vector2f(target.x - origin.x, target.y - origin.y);
    nBullet->bullet.setPosition(sf::Vector2f(origin.x-5, origin.y-5));

    float magnitude = std::sqrt(nBullet->direction.x * nBullet->direction.x + nBullet->direction.y * nBullet->direction.y);
    if (magnitude != 0){
        nBullet->direction.x /= magnitude;
        nBullet->direction.y /= magnitude;
    }

    active_bullets.push_back(nBullet);

    bulletsFired+=1;
}

void Player::registerEnemy(std::shared_ptr<Enemy> nEnemy){
    enemies.push_back(nEnemy);
}
