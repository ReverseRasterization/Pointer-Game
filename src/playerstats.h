#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

class PlayerStats {

    private:
        std::shared_ptr<sf::Text> scoreLabel;
        std::shared_ptr<sf::Text> accuracyLabel;

        int score;
        int accuracy;

    public:
        PlayerStats(sf::Font& font, sf::Vector2f windowSize);

        void changeScore(int factor);
        void setAccuracy(int bullets_fired, int bullets_hit);

        void draw(sf::RenderWindow& window);
        void adjustToWindowSize(sf::Vector2f nSize);
};