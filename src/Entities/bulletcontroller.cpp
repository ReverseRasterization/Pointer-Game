#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "bulletcontroller.h"


void BulletController::fireBullet(sf::Vector2f origin, sf::Vector2i target){
    Bullet nBullet;

    nBullet.direction = sf::Vector2f(target.x - origin.x, target.y - origin.y);
    nBullet.bullet.setPosition(sf::Vector2f(origin.x-5, origin.y-5));

    float magnitude = std::sqrt(nBullet.direction.x * nBullet.direction.x + nBullet.direction.y * nBullet.direction.y);
    if (magnitude != 0){
        nBullet.direction.x /= magnitude;
        nBullet.direction.y /= magnitude;
    }
}

void BulletController::renderBullets(float deltaTime, sf::RenderWindow& window){
    return;
}