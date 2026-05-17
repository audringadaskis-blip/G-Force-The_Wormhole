#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Constants.h"

class Player;

class Enemy {
public:
    sf::Texture textureRight1;
    sf::Texture textureRight2;
    sf::Texture textureLeft1;
    sf::Texture textureLeft2;
    std::unique_ptr<sf::Sprite> shape; 
    float speed;
    bool isActive;
    int direction;  // 1 = right, -1 = left
    
    float animationTimer;
    int currentFrame;

    Enemy();
    bool loadTextures();
    void spawn(float x, float y, int dir);
    void updateAnimation(float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOffScreen(float viewY) const;
};