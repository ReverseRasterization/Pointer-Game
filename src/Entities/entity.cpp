#include "entity.h"
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <iostream>

Entity::Entity(int hp, int max_hp, bool show_health_bar, std::string death_sound_directory, std::string hit_sound_directory): hp(hp), maxhp(max_hp), showHealthBar(show_health_bar) {
    if(!death_sound_directory.empty()){
        if(!deathSound_buffer.loadFromFile(death_sound_directory)) {
            std::cerr << "Failed to load death sound";
            return;
        }

        sf::Sound nDeathSound(deathSound_buffer);

        deathSound = std::make_shared<sf::Sound>(nDeathSound);
    }

    if(!hit_sound_directory.empty()){
        if(!hitSound_buffer.loadFromFile(hit_sound_directory)) {
            std::cerr << "Failed to load hit sound";
            return;
        }

        sf::Sound nHitSound(hitSound_buffer);

        hitSound = std::make_shared<sf::Sound>(nHitSound);
    }
}

void Entity::updateHealthBar() {
    if (showHealthBar) {
        healthBarFG->setSize(sf::Vector2f(healthBarBG->getSize().x * (static_cast<float>(hp)/100), healthBarFG->getSize().y));
    }
}

std::vector<int> Entity::getHitBox() {
    return std::vector<int>{xLeft, xRight, yTop, yBottom}; 
}

void Entity::setHitBox(int x_left, int x_right, int y_top, int y_bottom){
    xLeft = x_left;
    xRight = x_right;
    yTop = y_top;
    yBottom = y_bottom;
}

int Entity::getHealth(){
    return hp;
}

void Entity::takeDamage(int damage) {
    hp-=damage;

    if(hp <= 0){
        hp = 0;

        if (deathSound){
            deathSound->play();
        }
    }

    updateHealthBar();
}

void Entity::heal(int factor) {
    if (factor < 0) {
        hp = maxhp;
    }else {
        hp+=factor;
    }

    updateHealthBar();
}

void Entity::toggleHealthBar(bool toggle) {
    showHealthBar = toggle;
}

void Entity::constructHealthBar(sf::Vector2f size, sf::Vector2f position) {
    healthBarBG = std::make_shared<sf::RectangleShape>(size);
    healthBarFG = std::make_shared<sf::RectangleShape>(size);

    setHealthBarPosition(position);
    updateHealthBar();
}

void Entity::setHealthBarColor(sf::Color color) {
    if (!showHealthBar) {return;}

    healthBarFG->setFillColor(color);
}

void Entity::setHealthBarPosition(sf::Vector2f nPos) {
    if (!showHealthBar) {return;}

    healthBarBG->setPosition(nPos);
    healthBarFG->setPosition(nPos);
}

void Entity::setHealthBarSize(sf::Vector2f size) {
    if (!showHealthBar) {return;}

    healthBarBG->setSize(size);
    updateHealthBar();
}

std::vector<std::shared_ptr<sf::RectangleShape>> Entity::getHealthBar() {
    return {healthBarBG, healthBarFG};
}

bool Entity::hit(int target_x, int target_y){
    if (getHealth() == 0) {return false;};

    bool hit = target_x > xLeft && target_x < xRight && target_y > yTop && target_y < yBottom;

    if (hitSound && getHealth() > 0) {
        hitSound->play();
    }
    return hit;
}





