#ifndef ENEMY
#define ENEMY

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "entity.h"

class Enemy: public Entity
{
    private:
        int speed = 10;

        sf::RectangleShape entity = sf::RectangleShape({96.f, 80.f});

    public:
        Enemy(sf::Texture& enemyTexture);

        void draw(sf::RenderWindow& window);

        
        void kill();
};

#endif