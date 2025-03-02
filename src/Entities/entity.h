#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

// CLASS NOTES:
//      THE HEALTH BAR MUST BE MANUALLY CONSTRUCTED IN THE ENTITIES CONSTRUCTOR FUNCTION
//      THE DEATH OF AN ENTITY SHOULD BE HANDLED BY THE ENTITY ITSELF
//      THE HIT BOX BOUNDARIES SHOULD BE HANDLED BY THE ENTITY ITSELF

class Entity {
    private:
        int hp;
        int maxhp;

        int yTop;
        int yBottom;
        int xLeft;
        int xRight;

        std::shared_ptr<sf::RectangleShape> healthBarBG;
        std::shared_ptr<sf::RectangleShape> healthBarFG;

        std::shared_ptr<sf::Sound> deathSound;
        sf::SoundBuffer deathSound_buffer;

        std::shared_ptr<sf::Sound> hitSound;
        sf::SoundBuffer hitSound_buffer;

        bool showHealthBar = true;

    public:
        std::string id;

        void setHitBox(int x_left, int x_right, int y_top, int y_bottom);
        std::vector<int> getHitBox(); // x left, x right, y top, y bottom

        int getHealth();
        void takeDamage(int damage);
        void heal(int factor); // if the factor is <0, that means the entities health will be fully replenished

        void toggleHealthBar(bool toggle);
        void constructHealthBar(sf::Vector2f size, sf::Vector2f position);
        void setHealthBarColor(sf::Color color);
        void setHealthBarPosition(sf::Vector2f nPos);
        void setHealthBarSize(sf::Vector2f size);
        void updateHealthBar(); // note: this only updates the actual bar itself, not the background
        std::vector<std::shared_ptr<sf::RectangleShape>> getHealthBar();

        bool hit (int target_x, int target_y);

        virtual void draw(sf::RenderWindow& window) = 0;

        Entity(int hp, int max_hp, bool show_health_bar, std::string death_sound_directory, std::string hit_sound_directory);
};