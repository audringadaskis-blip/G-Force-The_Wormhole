#include "Player.h"

Player::Player() {
    shape.setSize({PLAYER_SIZE, PLAYER_SIZE});
    shape.setFillColor(COLOR_PLAYER);
    shape.setPosition({PLAYER_START_X, PLAYER_START_Y});
}

void Player::update(float dt, bool moveLeft, bool moveRight) {
    if (moveLeft) shape.move({-MOVE_SPEED * dt, 0.f});
    if (moveRight) shape.move({MOVE_SPEED * dt, 0.f});
}

void Player::setPosition(float x, float y) {
    shape.setPosition({x, y});
}

sf::Vector2f Player::getPosition() const {
    return shape.getPosition();
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}