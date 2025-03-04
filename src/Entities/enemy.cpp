#include "enemy.h"
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

Enemy::Enemy(sf::Texture& enemyTexture, sf::Vector2i x_bounds, sf::Vector2i y_bounds, std::string death_sound_directory, std::string hit_sound_directory, EntityManager& em): em(em)
{

    if(!death_sound_directory.empty()){
        if(!deathSound_buffer->loadFromFile(death_sound_directory)) {
            std::cerr << "Failed to load death sound";
            return;
        }

        deathSound = std::make_shared<sf::Sound>(*deathSound_buffer);
    }

    if(!hit_sound_directory.empty()){
        if(!hitSound_buffer->loadFromFile(hit_sound_directory)) {
            std::cerr << "Failed to load hit sound";
            return;
        }

        hitSound = std::make_shared<sf::Sound>(*hitSound_buffer);
    }
    
    sf::Vector2f position = choosePosition(x_bounds.x, x_bounds.y, y_bounds.x, y_bounds.y, {500, 500}, 200);
    healthBarBG.setPosition({position.x, position.y-30.f});
    healthBarFG.setPosition({position.x, position.y-30.f});
    healthBarFG.setFillColor(sf::Color(0,255,0));

    entity.setPosition(position);

    xLeft = position.x;
    xRight = position.x + entity.getSize().x;
    yTop = position.y;
    yBottom = position.y + entity.getSize().y;

    entity.setTexture(&enemyTexture);
}

void Enemy::updateHealthBar() {
    healthBarFG.setSize(sf::Vector2f(healthBarBG.getSize().x * (static_cast<float>(hp)/100), healthBarFG.getSize().y));
    
}

void Enemy::takeDamage(int damage) {
    hp-=damage;

    if(hp <= 0){
        hp = 0;

        if (deathSound){
            deathSound->play();
            em.killEntity(*this);
        }
    }

    updateHealthBar();
}

void Enemy::heal(int factor) {
    if (factor < 0) {
        hp = maxhp;
    }else {
        hp+=factor;
    }

    updateHealthBar();
}

int Enemy::getHealth(){
    return hp;
}

bool Enemy::hit(int target_x, int target_y){
    if (hp == 0) {return false;};

    bool hit = target_x > xLeft && target_x < xRight && target_y > yTop && target_y < yBottom;

    if (hitSound && hp > 0 && hit) {
        hitSound->play();
    }
    return hit;
}

void Enemy::draw(sf::RenderWindow& window){
    if (hp == 0){return;}

    window.draw(entity);
    window.draw(healthBarBG);
    window.draw(healthBarFG);
}





