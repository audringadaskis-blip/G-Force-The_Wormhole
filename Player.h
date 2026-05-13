#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Constants.h"

class Player {
public:
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> sprite;
    bool textureLoaded;
    
    Player();
    bool loadTexture(const std::string& path);
    void update(float dt, bool moveLeft, bool moveRight);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void setViewOffset(const sf::Vector2f& offset);
    sf::Vector2f getViewOffset() const;
    void draw(sf::RenderWindow& window);
    void reset();
};