#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "enemy.h"
#include "../playerstats.h"

class Player
{
    private:
        sf::ConvexShape pointer;

        sf::Sound& gunshot;

        struct Bullet{
            sf::Vector2f direction;
        
            float speed = 2000;
            int base_damage = 15;
        
            sf::CircleShape bullet = sf::CircleShape(5.0f);
        };

        std::vector<std::shared_ptr<Bullet>> active_bullets;
        std::vector<std::shared_ptr<Enemy>> enemies;

        PlayerStats& playerStats;

        // Accuracy
        int bulletsFired = 0;
        int bulletsHit = 0;

    public:
        Player(sf::Sound& gunshotSound, PlayerStats& playerStats);

        void pointTo(sf::Vector2f point);
        void adjust(sf::Vector2f nSize);
        void draw(sf::RenderWindow& window, float deltaTime);

        void fireBullet(sf::Vector2i target);
        void registerEnemy(std::shared_ptr<Enemy> nEnemy);

};

#endif