#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "enemy.h"
#include "entity.h"
#include "../playerstats.h"
#include "../entitymanager.h"

class Player
{
    private:
        sf::ConvexShape pointer;

        std::shared_ptr<sf::Sound> gunshot;
        sf::SoundBuffer gunshot_buffer;

        struct Bullet{
            sf::Vector2f direction;
        
            float speed = 2000;
            int base_damage = 15;
        
            sf::CircleShape bullet = sf::CircleShape(5.0f);
        };

        std::vector<std::shared_ptr<Bullet>> active_bullets;

        EntityManager& em;

        PlayerStats& playerStats;
        

        // Accuracy
        int bulletsFired = 0;
        int bulletsHit = 0;

    public:
        Player(PlayerStats& playerStats, EntityManager& em);

        void pointTo(sf::Vector2f point);
        void adjust(sf::Vector2f nSize);
        void draw(sf::RenderWindow& window, float deltaTime);

        void fireBullet(sf::Vector2i target);

};

#endif