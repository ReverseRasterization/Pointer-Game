#include "enemy.h"
#include "entity.h"
#include "../entitymanager.h"
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

sf::Vector2f choosePosition(int x_min, int x_max, int y_min, int y_max, sf::Vector2f playerPosition, int distance_from_player) {
    sf::Vector2f position(500, 500);

    while (getDistance(500, position.x, 500, position.y) < distance_from_player)
    {
        position = sf::Vector2f(x_min + rand()%x_max, y_min + rand()%y_max);
    }

    return position;
}

Enemy::Enemy(sf::Texture& enemyTexture, sf::Vector2i x_bounds, sf::Vector2i y_bounds, EntityManager& em): Entity(100, 100, true, "assets/Sounds/enemydied.wav", "assets/Sounds/hit.wav", em)
{
    
    sf::Vector2f position = choosePosition(x_bounds.x, x_bounds.y, y_bounds.x, y_bounds.y, {500, 500}, 200);
    constructHealthBar({96.f, 10.f}, {position.x, position.y-30.f});

    entity.setPosition(position);
    setHealthBarColor(sf::Color(0,255,0));

    setHitBox(position.x, position.x + 128, position.y, position.y + 107);

    entity.setTexture(&enemyTexture);

}

void Enemy::draw(sf::RenderWindow& window){
    if (getHealth() == 0){return;}

    window.draw(entity);

    auto healthBar = getHealthBar();
    window.draw(*healthBar[0]);
    window.draw(*healthBar[1]);
}





