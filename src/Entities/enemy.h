#ifndef ENEMY
#define ENEMY

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "entity.h"
#include "../entitymanager.h"

class Enemy: public Entity
{
    private:
        int speed = 10;

        sf::RectangleShape entity = sf::RectangleShape({96.f, 80.f});

        
    public:
        Enemy(sf::Texture& enemyTexture, sf::Vector2i x_bounds, sf::Vector2i y_bounds, EntityManager& em);

        void draw(sf::RenderWindow& window) override;
};

#endif