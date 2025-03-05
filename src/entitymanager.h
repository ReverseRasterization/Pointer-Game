#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include "playerstats.h"

class Enemy;

class EntityManager {
    private:

        PlayerStats& playerStats;

        std::vector<std::shared_ptr<Enemy>> entities;

        sf::Texture enemyTexture;

        std::shared_ptr<sf::Sound> deathSound;
        std::shared_ptr<sf::SoundBuffer> deathSound_buffer = std::make_shared<sf::SoundBuffer>();

        std::vector<int> enemyBounds; // x_min, x_max, y_min, y_max

        int maxEnemies = 100000000;

    public:

        EntityManager(std::string enemy_texture_directory, std::string death_sound_directory, PlayerStats& player_stats);

        void spawnEnemy(int health, std::string hit_sound_directory);
        void killEntity(Enemy& target_entity);

        std::vector<std::shared_ptr<Enemy>> getEntities();
        int getEntityCount();

        void setMaxEnemies(int max_enemies);
        void setEnemySpawnBoundaries(int x_min, int x_max, int y_min, int y_max);

        void drawEntities(sf::RenderWindow& window);

        
};