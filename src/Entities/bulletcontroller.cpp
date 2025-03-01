#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <memory>
#include <cmath>

#include "bulletcontroller.h"

void BulletController::fireBullet(sf::Vector2f origin, sf::Vector2i target){
    
}

void BulletController::renderBullets(float deltaTime, sf::RenderWindow& window){
    

        // std::vector<int> enemyBounds = enemy.getHitBox();

        // if (enemy.hit(sf::Vector2f(newX, newY))){
        //     hitsLanded+=1;

        //     float damage = it->base_damage;
        //     float damage_modifier = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f;

        //     if (damage_modifier < .3){
        //         damage_modifier = .3;
        //     }

        //     damage*=damage_modifier;

        //     enemy.takeDamage(static_cast<int>(damage), damage_modifier);

        //     

        //     if (enemy.getHp() <= 0) {
        //         eDeathSound.play();
        //         enemy = Enemy(ENEMY_HEALTH, enemyTexture);

        //         score+=15;
        //     }else {
        //         hitSound.play();
        //     }

        //     updateScore(scoreLabel, static_cast<sf::Vector2f>(window.getSize()), score);
        //     updateAccuracy(accuracyLabel, bulletsFired, hitsLanded);
            
        //     it = active_bullets.erase(it);
        //     break;
        // }

        // if (newX < 0 || newX > window.getSize().x || newY < 0 || newY > window.getSize().y){
        //     it = active_bullets.erase(it);
        //     updateAccuracy(accuracyLabel, bulletsFired, hitsLanded);
        //     break;
        // }else {
        //     window.draw(it->bullet);
        //     ++it;
        // }
    }
}