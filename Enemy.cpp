#include "Enemy.h"
#include <iostream>

Enemy::Enemy() : speed(WORM_SPEED), isActive(false), direction(1), 
                 animationTimer(0.f), currentFrame(0) {
    if (!loadTextures()) {
        sf::Image img;
        img.resize({static_cast<unsigned int>(WORM_WIDTH), 
                    static_cast<unsigned int>(WORM_HEIGHT)}, 
                   sf::Color(WORM_COLOR_R, WORM_COLOR_G, WORM_COLOR_B));
        
        if (!textureRight1.loadFromImage(img)) {
            std::cerr << "ERROR: Failed to create fallback texture\n";
            return;
        }
        textureRight2 = textureRight1;
        textureLeft1 = textureRight1;
        textureLeft2 = textureRight1;
        
        std::cout << "INFO: Using fallback worm rectangle\n";
    }
    
    shape = std::make_unique<sf::Sprite>(textureRight1);
    shape->setPosition({-100.f, -100.f});
    
    sf::Vector2u texSize = textureRight1.getSize();
    float scaleX = WORM_WIDTH / static_cast<float>(texSize.x);
    float scaleY = WORM_HEIGHT / static_cast<float>(texSize.y);
    shape->setScale({scaleX, scaleY});
}

bool Enemy::loadTextures() {
    bool loaded = true;
    loaded &= textureRight1.loadFromFile(WORM_RIGHT_1);
    loaded &= textureRight2.loadFromFile(WORM_RIGHT_2);
    loaded &= textureLeft1.loadFromFile(WORM_LEFT_1);
    loaded &= textureLeft2.loadFromFile(WORM_LEFT_2);
    
    if (loaded) {
        sf::Vector2u texSize = textureRight1.getSize();
        float scaleX = WORM_WIDTH / static_cast<float>(texSize.x);
        float scaleY = WORM_HEIGHT / static_cast<float>(texSize.y);
        if (shape) {
            shape->setScale({scaleX, scaleY});
        }
        return true;
    }
    
    std::cerr << "ERROR: Could not load all worm textures\n";
    return false;
}

void Enemy::spawn(float x, float y, int dir) {
    if (!shape) return;
    
    shape->setPosition({x, y});
    direction = dir;
    isActive = true;
    animationTimer = 0.f;
    currentFrame = 0;
    
    if (direction == 1) {
        shape->setTexture(textureRight1);
    } else {
        shape->setTexture(textureLeft1);
    }
}

void Enemy::updateAnimation(float dt) {
    if (!shape || !isActive) return;
    
    animationTimer += dt;
    
    if (animationTimer >= WORM_ANIMATION_SPEED) {
        animationTimer = 0.f;
        currentFrame = 1 - currentFrame;  // Switch between 0 and 1
        
        if (direction == 1) {
            if (currentFrame == 0) {
                shape->setTexture(textureRight1);
            } else {
                shape->setTexture(textureRight2);
            }
        } else {
            if (currentFrame == 0) {
                shape->setTexture(textureLeft1);
            } else {
                shape->setTexture(textureLeft2);
            }
        }
    }
}

// 1 -> right, -1 - > left
void Enemy::update(float dt) {
    if (!isActive || !shape) return;

    shape->move({direction * speed * dt, 0.f});
    
    sf::Vector2f pos = shape->getPosition();
    sf::FloatRect bounds = shape->getGlobalBounds();
    if (pos.x <= TUNNEL_WALL_WIDTH + 5.f) {
        direction = 1;
    } else if (pos.x >= WINDOW_WIDTH - TUNNEL_WALL_WIDTH - bounds.size.x - 5.f) {
        direction = -1;
    }
    
    updateAnimation(dt);
}

void Enemy::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(*shape);
    }
}

sf::FloatRect Enemy::getBounds() const {
    return shape->getGlobalBounds();
}

bool Enemy::isOffScreen(float viewY) const {
    if (!isActive) return true;
    float enemyY = shape->getPosition().y;
    return (enemyY < viewY - 400.f || enemyY > viewY + 400.f);
}