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

sf::Vector2f adjustSizeToAspectRatio(sf::Vector2f size, float target_aspect_ratio) {
    float originalAspectRatio = size.x/size.y;

    if (originalAspectRatio > target_aspect_ratio) { // the width is too large
        size.x = size.y * target_aspect_ratio;
        originalAspectRatio = size.x/size.y;
    }
    
    if (originalAspectRatio < target_aspect_ratio) { // the height is too large
        size.y = size.x * (80.f/96.f);
    }

    return size;
}

sf::Vector2f choosePosition(int x_min, int x_max, int y_min, int y_max, sf::Vector2f playerPosition, int distance_from_player) {
    sf::Vector2f position(500, 500);

    while (getDistance(500, position.x, 500, position.y) < distance_from_player)
    {
        position = sf::Vector2f(x_min + rand()%x_max, y_min + rand()%y_max);
    }

    return position;
}

Enemy::Enemy(int health, sf::Texture& enemyTexture, sf::Vector2i x_bounds, sf::Vector2i y_bounds, std::string hit_sound_directory, EntityManager& em): em(em), maxhp(health), hp(health)
{
    if(!hit_sound_directory.empty()){
        if(!hitSound_buffer->loadFromFile(hit_sound_directory)) {
            std::cerr << "Failed to load hit sound";
            return;
        }

        hitSound = std::make_shared<sf::Sound>(*hitSound_buffer);
    }

    float boundWidth = x_bounds.y - x_bounds.x;
    float boundHeight = y_bounds.y - y_bounds.x;

    sf::Vector2f position = choosePosition(x_bounds.x, x_bounds.y, y_bounds.x, y_bounds.y, {500, 500}, 200);
    enemyScalingData.positionScale = {position.x/boundWidth, position.y/boundHeight};
    entity.setPosition(position);

    updateToField({x_bounds.y-x_bounds.x, y_bounds.y - y_bounds.x});

    healthBarFG.setFillColor(sf::Color(0,255,0));
    entity.setTexture(&enemyTexture);
}

void Enemy::updateHealthBar() {
    healthBarFG.setSize(sf::Vector2f(healthBarBG.getSize().x * (static_cast<float>(hp)/maxhp), healthBarBG.getSize().y));
}

void Enemy::takeDamage(int damage) {
    hp-=damage;

    if(hp <= 0){
        hp = 0;
        em.killEntity(*this);
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

    sf::Vector2f position = entity.getPosition();
    sf::Vector2f size = entity.getSize();

    bool hit = target_x > position.x && target_x < position.x + size.x && target_y > position.y && target_y < position.y + size.y;

    if (hitSound && hp > 0 && hit) {
        hitSound->play();
    }
    return hit;
}

void Enemy::updateToField(sf::Vector2i nSize) {
    float boundWidth = nSize.x;
    float boundHeight = nSize.y;

    float aspect_ratio = boundWidth/boundHeight;

    sf::Vector2f enemy_size = adjustSizeToAspectRatio({boundWidth*enemyScalingData.sizeScale.x, boundHeight*enemyScalingData.sizeScale.y}, DISPLAY_ASPECT_RATIO);
    entity.setSize(enemy_size);

    sf::Vector2f enemy_position = {boundWidth * enemyScalingData.positionScale.x, boundHeight * enemyScalingData.positionScale.y};

    entity.setPosition(enemy_position);

    healthBarBG.setSize({enemy_size.x, nSize.y*.0125f});
    updateHealthBar();
    healthBarBG.setPosition({enemy_position.x, enemy_position.y-(enemy_size.y* .20f)});
    healthBarFG.setPosition({enemy_position.x, enemy_position.y-(enemy_size.y* .20f)});
}

void Enemy::draw(sf::RenderWindow& window){
    if (hp == 0){return;}

    window.draw(entity);
    window.draw(healthBarBG);
    window.draw(healthBarFG);
}





