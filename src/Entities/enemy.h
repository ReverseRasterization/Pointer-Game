#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "../entitymanager.h"


class Enemy
{
    private:
        int speed = 10;

        int hp;
        int maxhp;

        const float DISPLAY_ASPECT_RATIO = 1.2;

        struct EnemyScalingData {
            sf::Vector2f sizeScale = {.106, .90}; //96x80 in a 1000x1000 screen
            sf::Vector2f positionScale;
        };

        EnemyScalingData enemyScalingData;

        sf::RectangleShape entity = sf::RectangleShape({96.f, 80.f});
        sf::RectangleShape healthBarBG = sf::RectangleShape({96.f, 10.f});
        sf::RectangleShape healthBarFG = sf::RectangleShape({96.f, 10.f});

        std::shared_ptr<sf::Sound> hitSound;
        std::shared_ptr<sf::SoundBuffer> hitSound_buffer = std::make_shared<sf::SoundBuffer>();

        EntityManager& em;
        
        void updateHealthBar();

    public:
        Enemy(int health, sf::Texture& enemyTexture, sf::Vector2i x_bounds, sf::Vector2i y_bounds, std::string hit_sound_directory, EntityManager& em);

        void takeDamage(int damage);
        void heal(int factor); // if the factor is <0, that means the entities health will be fully replenished
        int getHealth();

        void updateToField(sf::Vector2i nSize);

        bool hit (int target_x, int target_y);

        void draw(sf::RenderWindow& window);
};