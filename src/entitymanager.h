#pragma once

#include "Entities/entity.h"

#include <iostream>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>



class EntityManager {
    private:

        std::vector<std::shared_ptr<Entity>> entities;

    public:

        void registerEntity(std::shared_ptr<Entity> new_entity);
        void killEntity(Entity& target_entity);

        std::vector<std::shared_ptr<Entity>> getEntities();
        int getEntityCount();

        void drawEntities(sf::RenderWindow& window);
};