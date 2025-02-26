
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "player.h"
#include "bulletcontroller.h"


Player::Player(sf::Sound& gunshotSound):bulletEngine(BulletController()), gunshot(gunshotSound){
    pointer.setPointCount(3);
    pointer.setPoint(0, {0, -25}); // This is the tip
    pointer.setPoint(1, {25, 25});
    pointer.setPoint(2, {-25, 25});
    pointer.setFillColor(sf::Color::Green);
    pointer.setOrigin({0,0});
    pointer.setPosition({500, 500});

}

void Player::pointTo(sf::Vector2f point)
{
    sf::Vector2f TrianglePos = pointer.getPosition();
    double dX = (double)point.x - TrianglePos.x;
    double dY = (double)point.y - TrianglePos.y;

    pointer.setRotation(sf::degrees((atan2(dY, dX)) * (180.0f / 3.14159265359f)+90));
}

void Player::adjust(sf::Vector2f nSize){
    pointer.setPosition(nSize/2.f);
}

void Player::draw(sf::RenderWindow& window){
    window.draw(pointer);
}

void Player::fireBullet(sf::Vector2i target){
    gunshot.play();
    bulletEngine.fireBullet(pointer.getPosition(), target);
}