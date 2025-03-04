#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Entities/enemy.h"

#include <SFML/Graphics.hpp>



class EntityManager {
    private:

        std::vector<std::shared_ptr<Enemy>> entities;

    public:

        void registerEntity(std::shared_ptr<Enemy> new_entity);
        void killEntity(Enemy& target_entity);

        std::vector<std::shared_ptr<Enemy>> getEntities();
        int getEntityCount();

        void drawEntities(sf::RenderWindow& window);
};