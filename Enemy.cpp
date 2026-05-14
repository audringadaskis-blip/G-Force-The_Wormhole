#include "Enemy.h"
#include <iostream>

Enemy::Enemy() : speed(WORM_SPEED), isActive(false), direction(1) {
    sf::Image img;
    img.resize({static_cast<unsigned int>(WORM_WIDTH), 
                static_cast<unsigned int>(WORM_HEIGHT)}, 
            sf::Color(WORM_COLOR_R, WORM_COLOR_G, WORM_COLOR_B));

    bool loadResult = texture.loadFromImage(img);
    if (!loadResult) {
        std::cerr << "CRITICAL: Failed to create fallback texture!\n";
        return;
    }
    
    shape = std::make_unique<sf::Sprite>(texture);
    shape->setPosition({-100.f, -100.f});
    
    if (loadTexture("assets/worm.png")) 
        shape->setTexture(texture);
}

bool Enemy::loadTexture(const std::string& path) {
    sf::Texture tempTexture;
    if (tempTexture.loadFromFile(path)) {
        texture = tempTexture;
        shape->setTexture(texture);
        
        shape->setTextureRect(sf::IntRect({0, 0},
            static_cast<sf::Vector2i>(texture.getSize())));
        
        sf::Vector2u texSize = texture.getSize();
        float scaleX = WORM_WIDTH / static_cast<float>(texSize.x);
        float scaleY = WORM_HEIGHT / static_cast<float>(texSize.y);
        shape->setScale({scaleX, scaleY});
        
        return true;
    }
    std::cerr << "Warning: Could not load worm texture: " << path << "\n";
    return false;
}

void Enemy::spawn(float x, float y, int dir) {
    shape->setPosition({x, y});
    direction = dir;
    isActive = true;
}

void Enemy::update(float dt) {
    if (!isActive) return;

    // Move horizontally
    shape->move({direction * speed * dt, 0.f});
    
    // Reverse direction if hitting tunnel walls
    sf::Vector2f pos = shape->getPosition();
    sf::FloatRect bounds = shape->getGlobalBounds();
    if (pos.x <= TUNNEL_WALL_WIDTH + 5.f) {
        direction = 1;  // Move right
    } else if (pos.x >= WINDOW_WIDTH - TUNNEL_WALL_WIDTH - 
               bounds.size.x - 5.f) {
        direction = -1;  // Move left
    }
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