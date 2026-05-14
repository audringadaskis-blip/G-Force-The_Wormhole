#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Constants.h"

class Player;

class Enemy {
public:
    sf::Texture texture;
    std::unique_ptr<sf::Sprite> shape; 
    float speed;
    bool isActive;
    int direction;  // 1 = right, -1 = left
    
    Enemy();
    bool loadTexture(const std::string& path);
    void spawn(float x, float y, int dir);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOffScreen(float viewY) const;
};