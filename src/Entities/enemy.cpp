#include "enemy.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

float getDistance(int x1, int x2, int y1, int y2);

Enemy::Enemy(int health, sf::Texture& enemyTexture): hp(health) 
{
    maxHp = health;

    sf::Vector2f position(500, 500);

    while (getDistance(500, position.x, 500, position.y) < 200)
    {
        position = sf::Vector2f(rand()%800, 100 + rand()%700);
    }

    entity.setPosition(position);
    healthBarBG.setPosition(sf::Vector2f(position.x, position.y-30));
    healthBarFG.setPosition(healthBarBG.getPosition());

    healthBarFG.setSize(sf::Vector2f(96.f, 10));
    healthBarFG.setFillColor(sf::Color(0,255,0));

    y_top = position.y;
    y_bottom = position.y+107;

    x_left = position.x;
    x_right = position.x + 128;

    entity.setTexture(&enemyTexture);
}

float getDistance(int x1, int x2, int y1, int y2){ // utilizes c^2 = a^2 + b^2 to determine distance
    // a = x diff
    // b = y diff

    int a = std::abs(x2-x1);
    int b = std::abs(y2-y1);

    return sqrt((a*a)+(b*b));
}

int Enemy::getHp(){
    return hp;
}

void Enemy::takeDamage(int damage, float damage_modifier){
    hp-=damage;

    if(hp < 0){
        hp = 0;
    }

    healthBarFG.setSize(sf::Vector2f(healthBarBG.getSize().x * (static_cast<float>(hp)/100), healthBarFG.getSize().y));
}

void Enemy::draw(sf::RenderWindow& window){
    window.draw(entity);
    window.draw(healthBarBG);
    window.draw(healthBarFG);
}

bool Enemy::hit(sf::Vector2f bulletPos){
    return bulletPos.x > x_left && bulletPos.x < x_right && bulletPos.y > y_top && bulletPos.y < y_bottom;
}

std::vector<int> Enemy::getHitBox() {
    return std::vector<int>{x_left, x_right, y_top, y_bottom};
}

