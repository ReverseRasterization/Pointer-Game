#ifndef ENEMY
#define ENEMY

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Enemy
{
    private:
        sf::Font font;

        int hp;
        int maxHp;
        int speed = 10;

        // Some hitbox data, the rectangles getPosition function is at the top left corner of the rectangle
        int y_top;
        int y_bottom;
        int x_left;
        int x_right;

        sf::RectangleShape entity = sf::RectangleShape({96.f, 80.f});
        sf::RectangleShape healthBarBG = sf::RectangleShape({96.f, 10});
        sf::RectangleShape healthBarFG = sf::RectangleShape({96.f, 10});

    public:
        Enemy(int health, sf::Texture& enemyTexture);

        int getHp();
        void takeDamage(int damage, float damage_modifier);
        void summon();
        void draw(sf::RenderWindow& window);
        bool hit(sf::Vector2f bulletPos);

        std::vector<sf::RectangleShape> getRenderObjects(); // entity, health bar BG, health bar FG
        std::vector<int> getHitBox(); // x left, x right, y top, y bottom
};

#endif