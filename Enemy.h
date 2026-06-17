#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Constants.h"

class Player;

class Enemy {
public:
    static sf::Texture textureRight1;
    static sf::Texture textureRight2;
    static sf::Texture textureLeft1;
    static sf::Texture textureLeft2;
    
    static sf::Texture textureBatRight1;
    static sf::Texture textureBatRight2;
    static sf::Texture textureBatRight3;
    static sf::Texture textureBatLeft1;
    static sf::Texture textureBatLeft2;
    static sf::Texture textureBatLeft3;

    std::unique_ptr<sf::Sprite> shape; 
    float speed;
    bool isActive;
    int direction;
    float animationTimer;
    int currentFrame;

    float baseScaleX;
    float baseScaleY;
    const sf::Texture* currentTexture;
    float actualWidth;
    float actualHeight;

    enum class Type { WORM, BAT };
    Type enemyType;

    Enemy();
    static bool loadTextures();
    void spawn(float x, float y, int dir, Type type);
    void updateAnimation(float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOffScreen(float viewY) const;
};