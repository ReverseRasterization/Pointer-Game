#include <SFML/Graphics.hpp>
#include "playerstats.h"

PlayerStats::PlayerStats(sf::Font& font, sf::Vector2f windowSize): score(0), accuracy(0) {
    sf::Text sl(font);
    sl.setCharacterSize(48);
    sl.setFillColor(sf::Color::White);
    sl.setString("Score: 0");

    sf::Text al(font);

    al.setFont(font);
    al.setCharacterSize(24);
    al.setFillColor(sf::Color::White);
    al.setString("Accuracy: Unknown");

    scoreLabel = std::make_shared<sf::Text>(sl);
    accuracyLabel = std::make_shared<sf::Text>(al);

    adjustToWindowSize(windowSize);
}

void PlayerStats::changeScore(int factor){
    score += factor;

    scoreLabel->setString("Score: " + std::to_string(score));
}

void PlayerStats::setAccuracy(int bullets_fired, int bullets_hit){
    if (bullets_fired == 0) {return;}
    float accuracy = (static_cast<float>(bullets_hit)/static_cast<float>(bullets_fired))*100.f;

    accuracyLabel->setString("Accuracy: " + std::to_string(static_cast<int>(accuracy)) + '%');
}

void PlayerStats::adjustToWindowSize(sf::Vector2f nSize) {
    scoreLabel->setOrigin(scoreLabel->getLocalBounds().getCenter());
    scoreLabel->setPosition({nSize.x/2.f,30});

    accuracyLabel->setPosition({10.f, nSize.y-30.f});
}

void PlayerStats::draw(sf::RenderWindow& window) {
    window.draw(*scoreLabel);
    window.draw(*accuracyLabel);    
}

