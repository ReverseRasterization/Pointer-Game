#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "player.h"
#include "enemy.h"
#include "../playerstats.h"
#include "../entitymanager.h"

sf::Vector2f adjustSizeToAspectRatio_p(sf::Vector2f size, float target_aspect_ratio) {
    float originalAspectRatio = size.x/size.y;

    if (originalAspectRatio > target_aspect_ratio) { // the width is too large
        size.x = size.y * target_aspect_ratio;
        originalAspectRatio = size.x/size.y;
    }
    
    if (originalAspectRatio < target_aspect_ratio) { // the height is too large
        size.y = size.x * (80.f/96.f);
    }

    return size;
}

Player::Player(PlayerStats& player_stats, EntityManager& em, std::vector<unsigned int> playing_bounds): playerStats(player_stats), em(em) {
    pointer.setPointCount(3);
    pointer.setFillColor(sf::Color::Green);
    adjust(playing_bounds);

    if(!gunshot_buffer.loadFromFile("assets/Sounds/gunshot.wav")) {
        std::cerr << "Failed to load gunshot sound";
        return;
    }

    sf::Sound nGunshot(gunshot_buffer);

    gunshot = std::make_shared<sf::Sound>(sf::Sound(nGunshot));
}

void Player::pointTo(sf::Vector2f point)
{
    sf::Vector2f TrianglePos = pointer.getPosition();
    double dX = (double)point.x - TrianglePos.x;
    double dY = (double)point.y - TrianglePos.y;

    pointer.setRotation(sf::degrees((atan2(dY, dX)) * (180.0f / 3.14159265359f)+90));
}

void Player::adjust(std::vector<unsigned int> playing_bounds){

    sf::Vector2f center = {(playing_bounds[0] + playing_bounds[1]) / 2.f, (playing_bounds[2] + playing_bounds[3]) / 2.f};
    sf::Vector2f size = {playing_bounds[1]-playing_bounds[0], playing_bounds[3]-playing_bounds[2]};
    pointer.setOrigin({0,0});
    pointer.setPosition(center);

    float sizeFactor = adjustSizeToAspectRatio_p({size.x*.025f, size.y*.025f}, 1).x;

    pointer.setPoint(0, {0, -sizeFactor}); // This is the tip
    pointer.setPoint(1, {sizeFactor, sizeFactor});
    pointer.setPoint(2, {-sizeFactor, sizeFactor});
}

void Player::draw(sf::RenderWindow& window, float deltaTime){
    // Draw the actual triangle
    window.draw(pointer);

    // Get the entities
    std::vector<std::shared_ptr<Enemy>> entityList = em.getEntities();

    // Draw all the bullets
    for (auto it = active_bullets.begin(); it != active_bullets.end();){
        // Determine its new position
        std::shared_ptr<Bullet> bulletptr = *it;

        float newX = bulletptr->bullet.getPosition().x + (bulletptr->direction.x * bulletptr->speed * deltaTime);
        float newY = bulletptr->bullet.getPosition().y + (bulletptr->direction.y * bulletptr->speed * deltaTime);

        bulletptr->bullet.setPosition(sf::Vector2f(newX, newY));

        // Check if it hit any enemies
        bool hit = false;
        for (auto enemy = entityList.begin(); enemy != entityList.end(); ++enemy){
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
    gunshot->play();

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