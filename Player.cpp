#include <iostream>
#include "Player.h"

Player::Player() : textureLoaded(false), facingLeft(false), animationTimer(0.f), animationFrame(false) {
    sf::Image img;
    img.resize({static_cast<unsigned int>(PLAYER_SIZE), 
                static_cast<unsigned int>(PLAYER_SIZE)}, 
               sf::Color::Green);
    
    if (!texture.loadFromImage(img)) {
        std::cerr << "CRITICAL: Failed to create fallback texture!\n";
        return;
    }
    
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setPosition({PLAYER_START_X, PLAYER_START_Y + SPRITE_OFFSET_Y});

    if (!left1.loadFromFile("assets/character_left1.png")) {
        std::cout << "could not load left1\n";
    }
    if (!left2.loadFromFile("assets/character_left2.png")) {
        std::cout << "could not load left2\n";
    }
    if (!right1.loadFromFile("assets/character_right1.png")) {
        std::cout << "could not load right1\n";
    }
    if (!right2.loadFromFile("assets/character_right2.png")) {
        std::cout << "could not load right2\n";
    }

    sprite->setTexture(right1);
    
    if (loadTexture(PLAYER_TEXTURE_PATH)) {
        sprite->setTexture(texture);
    } else {
        std::cout << "Info: Using fallback green square\n";
    }
}

bool Player::loadTexture(const std::string& path) {
    sf::Texture tempTexture;
    if (tempTexture.loadFromFile(path)) {
        texture = tempTexture;
        sprite->setTexture(texture);
        
        sprite->setTextureRect(sf::IntRect({0, 0}, 
            static_cast<sf::Vector2i>(texture.getSize())));
        
        textureLoaded = true;
        return true;
    }
    std::cerr << "Warning: Could not load texture: " << path << "\n";
    textureLoaded = false;
    return false;
}

void Player::update(float dt, bool moveLeft, bool moveRight) {

    bool moving = false;

    animationTimer += dt;

    if (animationTimer >= 0.20f) {
        animationFrame = !animationFrame;
        animationTimer = 0.f;
    }

    if (moveLeft) {
        sprite->move({-MOVE_SPEED * dt, 0.f});
        facingLeft = true;
        moving = true;

    }
    if (moveRight) {
        sprite->move({MOVE_SPEED * dt, 0.f});
        facingLeft = false;
        moving = true;
    }
        if (moving) {

        if (facingLeft) {

            if (animationFrame)
                sprite->setTexture(left1);
            else
                sprite->setTexture(left2);
        }
        else {

            if (animationFrame)
                sprite->setTexture(right1);
            else
                sprite->setTexture(right2);
        }

    } else {
        // Idle frame
        sprite->setTexture(texture);
    }
}

void Player::setPosition(float x, float y) {
    sprite->setPosition({x, y + SPRITE_OFFSET_Y});
}

sf::Vector2f Player::getPosition() const {
    return {sprite->getPosition().x, sprite->getPosition().y - SPRITE_OFFSET_Y};
}

void Player::setViewOffset(const sf::Vector2f& offset) { }

sf::Vector2f Player::getViewOffset() const {
    return {0.f, SPRITE_OFFSET_Y};
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}

void Player::reset() {
    sprite->setPosition({PLAYER_START_X, PLAYER_START_Y + SPRITE_OFFSET_Y});
}

bool Player::checkCollision(const sf::FloatRect& enemyBounds) const {
    if (!sprite) return false;
    
    sf::FloatRect playerBounds = sprite->getGlobalBounds();
    
    float playerShrink = 8.f;
    float enemyShrink = 9.f;
    
    float pLeft = playerBounds.position.x + playerShrink;
    float pTop = playerBounds.position.y + playerShrink;
    float pRight = playerBounds.position.x + playerBounds.size.x - playerShrink;
    float pBottom = playerBounds.position.y + playerBounds.size.y - playerShrink;
    
    float eLeft = enemyBounds.position.x + enemyShrink;
    float eTop = enemyBounds.position.y + enemyShrink;
    float eRight = enemyBounds.position.x + enemyBounds.size.x - enemyShrink;
    float eBottom = enemyBounds.position.y + enemyBounds.size.y - enemyShrink;
    
    // Check intersection
    return (pLeft < eRight && pRight > eLeft &&
            pTop < eBottom && pBottom > eTop);
}
