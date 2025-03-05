#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

class Enemy;

class EntityManager {
    private:

        std::vector<std::shared_ptr<Enemy>> entities;

    public:

        void registerEntity(std::shared_ptr<Enemy> new_entity);
        void killEntity(Enemy& target_entity);

        std::vector<std::shared_ptr<Enemy>> getEntities();
        int getEntityCount();

        void drawEntities(sf::RenderWindow& window);

        EntityManager();
};