#ifndef PLAYER
#define PLAYER

#include <SFML/Graphics.hpp>

class Player
{
    

    private:
        sf::ConvexShape pointer;

    public:
        struct Bullet{
            sf::Vector2f direction;
        
            float speed = 2000;
            int base_damage = 15;
        
            sf::CircleShape bullet = sf::CircleShape(5.0f);
        };

        Player();

        void pointTo(sf::Vector2f point);
        void adjust(sf::Vector2f nSize);
        void draw(sf::RenderWindow& window);

        Bullet fireBullet(sf::Vector2i target);


};

#endif