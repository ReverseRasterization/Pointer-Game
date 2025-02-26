#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player
{
    

    private:
        sf::ConvexShape pointer;
        sf::Sound gunshot;

    public:
        Player(std::string gunshotSoundPath);

        void pointTo(sf::Vector2f point);
        void adjust(sf::Vector2f nSize);
        void draw(sf::RenderWindow& window);

        void fireBullet(sf::Vector2i target);


};

#endif