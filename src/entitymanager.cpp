#include <iostream>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Entities/entity.h"
#include "entitymanager.h"

void EntityManager::registerEntity(std::shared_ptr<Entity> new_entity) {
    entities.push_back(new_entity);
} 

void EntityManager::killEntity(Entity& target_entity) { 
    for (auto it = entities.begin(); it != entities.end(); it++){
        if (it->get() == &target_entity) {
            entities.erase(it);
            break;
        }
    }
}

std::vector<std::shared_ptr<Entity>> EntityManager::getEntities() {
    return entities;
}

int EntityManager::getEntityCount() {
    return entities.size();
}

void EntityManager::drawEntities(sf::RenderWindow& window) {
    for (const auto& entity : entities){
        if (entity->getHealth() > 0) {
            entity->draw(window);
        }
    }
}