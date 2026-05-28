#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Constants.h"

class Enemy;

class Player {
public:
    sf::Texture texture;
    sf::Texture left1;
    sf::Texture left2;
    sf::Texture right1;
    sf::Texture right2;
    std::unique_ptr<sf::Sprite> sprite;
    bool textureLoaded;

    bool facingLeft;
    float animationTimer;
    bool animationFrame;
    
    Player();
    bool loadTexture(const std::string& path);
    void update(float dt, bool moveLeft, bool moveRight);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void setViewOffset(const sf::Vector2f& offset);
    sf::Vector2f getViewOffset() const;
    void draw(sf::RenderWindow& window);
    void reset();
    bool checkCollision(const sf::FloatRect& enemyBounds) const;
};
