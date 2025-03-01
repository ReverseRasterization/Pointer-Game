#include "enemy.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>

float getDistance(int x1, int x2, int y1, int y2){ // utilizes c^2 = a^2 + b^2 to determine distance
    // a = x diff
    // b = y diff

    int a = std::abs(x2-x1);
    int b = std::abs(y2-y1);

    return sqrt((a*a)+(b*b));
}

Enemy::Enemy(sf::Texture& enemyTexture): Entity(100, 100, true)
{
    sf::Vector2f position(500, 500);

    while (getDistance(500, position.x, 500, position.y) < 200)
    {
        position = sf::Vector2f(rand()%800, 100 + rand()%700);
    }

    constructHealthBar({96.f, 10.f}, {position.x, position.y-30.f});

    entity.setPosition(position);
    setHealthBarColor(sf::Color(0,255,0));

    setHitBox(position.x, position.x + 128, position.y, position.y + 107);

    entity.setTexture(&enemyTexture);
}


void Enemy::draw(sf::RenderWindow& window){
    window.draw(entity);

    auto healthBar = getHealthBar();
    window.draw(*healthBar[0]);
    window.draw(*healthBar[1]);
}





