#include "Enemy.h"
#include <iostream>

sf::Texture Enemy::textureRight1;
sf::Texture Enemy::textureRight2;
sf::Texture Enemy::textureLeft1;
sf::Texture Enemy::textureLeft2;

sf::Texture Enemy::textureBatRight1;
sf::Texture Enemy::textureBatRight2;
sf::Texture Enemy::textureBatRight3;
sf::Texture Enemy::textureBatLeft1;
sf::Texture Enemy::textureBatLeft2;
sf::Texture Enemy::textureBatLeft3;

static bool texturesLoaded = false;

Enemy::Enemy() : speed(WORM_SPEED), isActive(false), direction(1), 
                 animationTimer(0.f), currentFrame(0),
                 baseScaleX(1.f), baseScaleY(1.f),
                 currentTexture(nullptr),
                 enemyType(Type::WORM) {
    
    if (!texturesLoaded) {
        loadTextures();
        texturesLoaded = true;
    }
    
    shape = std::make_unique<sf::Sprite>(textureRight1);
    shape->setPosition({-100.f, -100.f});
    
    sf::Vector2u texSize = textureRight1.getSize();
    baseScaleX = WORM_WIDTH / static_cast<float>(texSize.x);
    baseScaleY = WORM_HEIGHT / static_cast<float>(texSize.y);
    shape->setScale({baseScaleX, baseScaleY});
}

bool Enemy::loadTextures() {
    bool loaded = true;
    loaded &= textureRight1.loadFromFile(WORM_LEFT_1);
    loaded &= textureRight2.loadFromFile(WORM_LEFT_2);
    loaded &= textureLeft1.loadFromFile(WORM_RIGHT_1);
    loaded &= textureLeft2.loadFromFile(WORM_RIGHT_2);
    
    loaded &= textureBatRight1.loadFromFile(BAT_RIGHT_1);
    loaded &= textureBatRight2.loadFromFile(BAT_RIGHT_2);
    loaded &= textureBatRight3.loadFromFile(BAT_RIGHT_3);
    loaded &= textureBatLeft1.loadFromFile(BAT_LEFT_1);
    loaded &= textureBatLeft2.loadFromFile(BAT_LEFT_2);
    loaded &= textureBatLeft3.loadFromFile(BAT_LEFT_3);

    if (loaded) {
        std::cout << "Worm textures loaded successfully!\n";
        return true;
    }
    
    std::cerr << "Warning: Could not load worm textures, using fallback\n";
    
    sf::Image img;
    img.resize({static_cast<unsigned int>(WORM_WIDTH), 
                static_cast<unsigned int>(WORM_HEIGHT)}, 
               sf::Color(WORM_COLOR_R, WORM_COLOR_G, WORM_COLOR_B));
    
    if (!textureRight1.loadFromImage(img)) {
        std::cerr << "ERROR: Failed to create fallback texture!\n";
        return false;
    }
    
    textureRight2 = textureRight1;
    textureLeft1 = textureRight1;
    textureLeft2 = textureRight1;
    
    return true;
}

void Enemy::spawn(float x, float y, int dir, Type type) {
    if (!shape) return;
    
    enemyType = type;

    shape->setPosition({x, y});
    
    direction = dir;
    isActive = true;
    animationTimer = 0.f;
    currentFrame = 0;
    
    if (type == Type::BAT) {
        speed = BAT_SPEED;
        sf::Vector2u texSize = textureBatRight1.getSize();
        actualWidth = static_cast<float>(texSize.x);
        actualHeight = static_cast<float>(texSize.y);
        
    } else {
        speed = WORM_SPEED;
        sf::Vector2u texSize = textureRight1.getSize();
        actualWidth = static_cast<float>(texSize.x);
        actualHeight = static_cast<float>(texSize.y);
    }
    
    shape->setOrigin({actualWidth / 2.f, actualHeight / 2.f});
    
    // Set initial texture
    if (type == Type::BAT) {
        currentTexture = (direction == 1) ? &textureBatRight1 : &textureBatLeft1;
    } else {
        currentTexture = (direction == 1) ? &textureRight1 : &textureLeft1;
    }
    shape->setTexture(*currentTexture);
}

void Enemy::updateAnimation(float dt) {
    if (!shape || !isActive) return;
    
    animationTimer += dt;
    
    float animSpeed = (enemyType == Type::BAT) ? BAT_ANIMATION_SPEED : WORM_ANIMATION_SPEED;
    int maxFrames = (enemyType == Type::BAT) ? 3 : 2;
    
    if (animationTimer >= animSpeed) {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % maxFrames;
        
        if (enemyType == Type::BAT) {
            if (direction == 1) {
                if (currentFrame == 0) currentTexture = &textureBatRight1;
                else if (currentFrame == 1) currentTexture = &textureBatRight2;
                else currentTexture = &textureBatRight3;
            } else {
                if (currentFrame == 0) currentTexture = &textureBatLeft1;
                else if (currentFrame == 1) currentTexture = &textureBatLeft2;
                else currentTexture = &textureBatLeft3;
            }
        } else {
            // WORM
            if (direction == 1) {
                if (currentFrame == 0) currentTexture = &textureRight1;
                else currentTexture = &textureRight2;
            } else {
                if (currentFrame == 0) currentTexture = &textureLeft1;
                else currentTexture = &textureLeft2;
            }
        }
        
        shape->setTexture(*currentTexture);
        
        sf::Vector2u texSize = currentTexture->getSize();
        shape->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(texSize.x, texSize.y)));
        
        float scaleX = (enemyType == Type::BAT) ? 
            (BAT_WIDTH / static_cast<float>(texSize.x)) :
            (WORM_WIDTH / static_cast<float>(texSize.x));
        float scaleY = (enemyType == Type::BAT) ? 
            (BAT_HEIGHT / static_cast<float>(texSize.y)) :
            (WORM_HEIGHT / static_cast<float>(texSize.y));
        
        shape->setScale(sf::Vector2f(scaleX, scaleY));
    }
}

void Enemy::update(float dt) {
    if (!isActive || !shape) return;

    shape->move({direction * speed * dt, 0.f});
    
    sf::FloatRect bounds = shape->getGlobalBounds();
    float leftEdge = bounds.position.x;
    float rightEdge = bounds.position.x + bounds.size.x;
    float topEdge = bounds.position.y;
    
    // Left wall
    if (leftEdge <= TUNNEL_WALL_WIDTH) {
        direction = 1;
        // Snap to wall
        shape->setPosition(sf::Vector2f(TUNNEL_WALL_WIDTH, topEdge));
    } 

    // Right wall
    else if (rightEdge >= WINDOW_WIDTH - TUNNEL_WALL_WIDTH) {
        direction = -1;
    }
    
    updateAnimation(dt);
}

void Enemy::draw(sf::RenderWindow& window) {
    if (isActive && shape) {
        window.draw(*shape);
    }
}

sf::FloatRect Enemy::getBounds() const {
    if (!shape) {
        return sf::FloatRect({0, 0}, {0, 0});
    }
    return shape->getGlobalBounds();
}

bool Enemy::isOffScreen(float viewY) const {
    if (!isActive || !shape) return true;
    float enemyY = shape->getPosition().y;
    return (enemyY < viewY - 400.f || enemyY > viewY + 400.f);
}