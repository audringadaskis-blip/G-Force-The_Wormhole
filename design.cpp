#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "Constants.h"
#include "Player.h"
#include "Enemy.h" 

constexpr float BLOCK_WIDTH = 40.f;
constexpr float BLOCK_HEIGHT = 20.f;
constexpr float TUNNEL_LEFT = 100.f;
constexpr float TUNNEL_RIGHT = 700.f;
constexpr int   BLOCKS_PER_ROW = static_cast<int>((TUNNEL_RIGHT - TUNNEL_LEFT) / BLOCK_WIDTH);

struct Block {
    float x, y;
    bool  broken = false;
    int breakCount = 0;

    sf::FloatRect getBounds() const {
        return sf::FloatRect({ x, y }, { BLOCK_WIDTH, BLOCK_HEIGHT });
    }
};

struct FallingPlatform {
    std::vector<Block> blocks;
    float y;
    bool  active = true;
    sf::Texture blockTexture;

};

std::vector<FallingPlatform> fallingPlatforms;

void generateFallingPlatforms(int count) {
    fallingPlatforms.clear();

    for (int i = 0; i < count; i++) {
        FallingPlatform p;
        p.y = TUNNEL_ENTRY_Y + 300.f + i * 220.f;

        if (!p.blockTexture.loadFromFile("assets/dirt2.png")) {
            std::cout << "Failed to load texture!\n";
        }

        p.blockTexture.setRepeated(false);

        for (int col = 0; col < BLOCKS_PER_ROW; col++) {
            Block b;
            b.x = TUNNEL_LEFT + col * BLOCK_WIDTH;
            b.y = p.y;
            p.blocks.push_back(b);
        }

        fallingPlatforms.push_back(p);
    }
}

void resetFallingPlatforms(int count)
{
    generateFallingPlatforms(count);
}

void drawFallingPlatforms(sf::RenderWindow& window, const sf::View& view, bool inTunnel) {
    if (!inTunnel) return;

    float viewTop    = view.getCenter().y - 300.f;
    float viewBottom = view.getCenter().y + 300.f;

    for (const auto& p : fallingPlatforms) {
        if (!p.active) continue;
        if (p.y < viewTop || p.y > viewBottom) continue;

        for (const auto& b : p.blocks) {
            if (b.broken) continue;

            sf::RectangleShape tile({ BLOCK_WIDTH, BLOCK_HEIGHT });
            tile.setTexture(&p.blockTexture);
            tile.setTextureRect(sf::IntRect({0, 0}, {(int)BLOCK_WIDTH, (int)BLOCK_HEIGHT}));
            tile.setPosition({ b.x, b.y });
            window.draw(tile);
        }
    }
}

bool checkFallingPlatformCollisions(float& playerY, float playerX, bool& isFalling, bool spaceJustPressed)
{
    float playerFeet = playerY + PLAYER_SIZE;

    for (auto& p : fallingPlatforms) {
        if (!p.active) continue;

        for (auto& b : p.blocks) {
            if (b.broken) continue;

            bool inXRange =
                playerX + PLAYER_SIZE > b.x &&
                playerX < b.x + BLOCK_WIDTH;

            if (!inXRange) continue;

            bool landingOnTop =
                playerFeet >= b.y &&
                playerFeet <= b.y + BLOCK_HEIGHT;

            if (landingOnTop) {

                playerY = b.y - PLAYER_SIZE;
                isFalling = false;

                if (spaceJustPressed) {
                    b.breakCount++;
                }

                // 3 paspaudimai = break
                if (b.breakCount >= 3) {
                    b.broken = true;
                    isFalling = true;
                }

                return true;
            }
        }
    }

    return false;
}

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
    static sf::Texture platformTexture;
    static bool loaded = false;
    if (!loaded) {
        if (!platformTexture.loadFromFile("assets/dirt2.png")) {
            std::cout << "Failed to load dirt2.png!\n";
        }
        platformTexture.setRepeated(true);
        loaded = true;
    }

    sf::RectangleShape left({HOLE_START_X, 20.f});
    sf::RectangleShape right({WINDOW_WIDTH - HOLE_END_X, 20.f});

    left.setTexture(&platformTexture);
    left.setTextureRect(sf::IntRect({0, 0}, {(int)HOLE_START_X, 20}));
    right.setTexture(&platformTexture);
    right.setTextureRect(sf::IntRect({0, 0}, {(int)(WINDOW_WIDTH - HOLE_END_X), 20}));

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

void drawFinalPlatform(sf::RenderWindow& window) {
    static sf::Texture platformTexture;
    static bool loaded = false;
    if (!loaded) {
        if (!platformTexture.loadFromFile("assets/dirt2.png")) {
            std::cout << "Failed to load dirt2.png!\n";
        }
        platformTexture.setRepeated(true);
        loaded = true;
    }

    sf::RectangleShape finalPlatform(sf::Vector2f(WINDOW_WIDTH, FINAL_PLATFORM_HEIGHT));
    finalPlatform.setPosition(sf::Vector2f(0.f, FINAL_PLATFORM_Y));
    finalPlatform.setTexture(&platformTexture);
    
    finalPlatform.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(WINDOW_WIDTH, FINAL_PLATFORM_HEIGHT)));
    
    window.draw(finalPlatform);
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

void drawEnemies(sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies) {    
    int activeCount = 0;
    for (auto& enemy : enemies) {
        if (enemy->isActive) {
            enemy->draw(window);
            activeCount++;
        }
    }
}
