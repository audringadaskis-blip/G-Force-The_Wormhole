#include <iostream>
#include "Player.h"

Player::Player() : textureLoaded(false) {
    sf::Image img;
    img.resize({static_cast<unsigned int>(PLAYER_SIZE), 
                static_cast<unsigned int>(PLAYER_SIZE)}, 
               sf::Color::Green);
    
    if (!texture.loadFromImage(img)) {
        std::cerr << "Error: Failed to create texture from image\n";
    }
    
    sprite = std::make_unique<sf::Sprite>(texture);
    sprite->setPosition({PLAYER_START_X, PLAYER_START_Y + SPRITE_OFFSET_Y});
    
    if (loadTexture(PLAYER_TEXTURE_PATH)) {
        sprite->setTexture(texture);
    }
}

bool Player::loadTexture(const std::string& path) {
    sf::Texture tempTexture;
    if (tempTexture.loadFromFile(path)) {
        texture = tempTexture;
        sprite->setTexture(texture);
        textureLoaded = true;
        return true;
    }
    std::cerr << "Warning: Could not load texture: " << path << "\n";
    textureLoaded = false;
    return false;
}

void Player::update(float dt, bool moveLeft, bool moveRight) {
    if (moveLeft) sprite->move({-MOVE_SPEED * dt, 0.f});
    if (moveRight) sprite->move({MOVE_SPEED * dt, 0.f});
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