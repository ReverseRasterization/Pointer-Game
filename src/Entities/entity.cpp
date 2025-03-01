#include "entity.h"
#include <vector>

Entity::Entity(int hp, int max_hp, bool show_health_bar): hp(hp), maxhp(max_hp), showHealthBar(show_health_bar) {}

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

void Entity::takeDamage(int damage) {
    hp-=damage;

    if(hp <= 0){
        hp = 0;
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
    return target_x > xLeft && target_x < xRight && target_y > yTop && target_y < yBottom;
}





