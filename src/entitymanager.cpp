#include <iostream>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Entities/enemy.h"
#include "entitymanager.h"

void EntityManager::spawnEnemy(int health, std::string hit_sound_directory) {
    if (entities.size() == maxEnemies) {return;}

    entities.push_back(std::make_shared<Enemy>(
        health, 
        enemyTexture, 
        sf::Vector2i(enemyBounds[0], enemyBounds[1]), 
        sf::Vector2i(enemyBounds[2], enemyBounds[3]),
        hit_sound_directory, 
        *this
    ));
} 

void EntityManager::killEntity(Enemy& target_entity) { 
    for (auto it = entities.begin(); it != entities.end(); it++){
        if (it->get() == &target_entity) {
            entities.erase(it);
            break;
        }
    }

    if (deathSound){
        deathSound->play();
    }

    playerStats.changeScore(15);
}

std::vector<std::shared_ptr<Enemy>> EntityManager::getEntities() {
    return entities;
}

int EntityManager::getEntityCount() {
    return entities.size();
}

void EntityManager::setMaxEnemies(int max_enemies) {
    maxEnemies = max_enemies;
}

void EntityManager::setEnemySpawnBoundaries(int x_min, int x_max, int y_min, int y_max) {
    enemyBounds = {x_min, x_max, y_min, y_max};

    for (const auto& entity : entities){
        if (entity->getHealth() > 0) {
            entity->updateToField({x_max-x_min, y_max-y_min});
        }
    }
}

void EntityManager::drawEntities(sf::RenderWindow& window) {
    for (const auto& entity : entities){
        if (entity->getHealth() > 0) {
            entity->draw(window);
        }
    }
}

EntityManager::EntityManager(std::string enemy_texture_directory, std::string death_sound_directory, PlayerStats& player_stats): playerStats(player_stats) {
    if (!enemyTexture.loadFromFile(enemy_texture_directory)){
        std::cerr << "Failed to load enemy texture";
        return;
    }

    if(!death_sound_directory.empty()){
        if(!deathSound_buffer->loadFromFile(death_sound_directory)) {
            std::cerr << "Failed to load death sound";
            return;
        }

        deathSound = std::make_shared<sf::Sound>(*deathSound_buffer);
    }
}