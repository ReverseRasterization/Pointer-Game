#ifndef BULLETCONTROLLER
#define BULLETCONTROLLER

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class BulletController
{
    private:
        struct Bullet{
            sf::Vector2f direction;
        
            float speed = 2000;
            int base_damage = 15;
        
            sf::CircleShape bullet = sf::CircleShape(5.0f);
        };

        std::vector<Bullet> active_bullets;

    public:
        void fireBullet(sf::Vector2f origin, sf::Vector2i target);
        void renderBullets(float deltaTime, sf::RenderWindow& window);
};

#endif