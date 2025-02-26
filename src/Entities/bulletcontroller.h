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

        sf::Sound hitSound;

    public:
        BulletController(std::string hitSoundPath);

        void fire();
        


    
        

};

#endif