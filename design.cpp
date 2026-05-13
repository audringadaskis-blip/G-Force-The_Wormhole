#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Player.h"

// Draw tunnel walls
void drawTunnel(sf::RenderWindow& window) {
    sf::RectangleShape leftWall({TUNNEL_WALL_WIDTH, TUNNEL_HEIGHT});
    sf::RectangleShape rightWall({TUNNEL_WALL_WIDTH, TUNNEL_HEIGHT});
    leftWall.setFillColor(COLOR_TUNNEL_WALL);
    rightWall.setFillColor(COLOR_TUNNEL_WALL);
    leftWall.setPosition({TUNNEL_LEFT_X, TUNNEL_OFFSET});
    rightWall.setPosition({TUNNEL_RIGHT_X, TUNNEL_OFFSET});
    window.draw(leftWall);
    window.draw(rightWall);
}

// Draw platform with hole
void drawPlatform(sf::RenderWindow& window) {
    sf::RectangleShape left({HOLE_START_X, 20.f});
    sf::RectangleShape right({WINDOW_WIDTH - HOLE_END_X, 20.f});
    left.setFillColor(COLOR_PLATFORM);
    right.setFillColor(COLOR_PLATFORM);
    left.setPosition({0.f, TERRAIN_Y});
    right.setPosition({HOLE_END_X, TERRAIN_Y});
    window.draw(left);
    window.draw(right);
}

// Draw dynamic rungs (only visible ones)
void drawRungs(sf::RenderWindow& window, const sf::View& view, bool inTunnel) {
    if (!inTunnel) return;
    
    float viewTop = view.getCenter().y - 300.f;
    float viewBottom = view.getCenter().y + 300.f;
    
    int startRung = static_cast<int>((viewTop - TUNNEL_ENTRY_Y) / RUNG_SPACING) - 1;
    int endRung = static_cast<int>((viewBottom - TUNNEL_ENTRY_Y) / RUNG_SPACING) + 1;
    
    for (int i = startRung; i <= endRung; i++) {
        if (i >= 0) {
            sf::RectangleShape rung({600.f, 3.f});
            rung.setFillColor(COLOR_RUNG);
            rung.setPosition({100.f, TUNNEL_ENTRY_Y + 50.f + i * RUNG_SPACING});
            window.draw(rung);
        }
    }
}

// Update camera view (smooth follow)
void updateCamera(sf::View& view, const sf::Vector2f& playerPos, 
                  bool inTunnel, bool isFalling, float dt) {
    float targetViewY = 300.f;
    
    if (playerPos.y > TUNNEL_ENTRY_Y) {
        targetViewY = playerPos.y - 50.f;
    }

    float currentViewY = view.getCenter().y;
    float newViewY = currentViewY + (targetViewY - currentViewY) * 5.0f * dt;
    view.setCenter({400.f, newViewY});
}

// Reset camera to default position
void resetCamera(sf::View& view) {
    view.setCenter({400.f, 300.f});
}