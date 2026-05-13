#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Player {
public:
    sf::RectangleShape shape;
    
    Player();
    void update(float dt, bool moveLeft, bool moveRight);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window);
};