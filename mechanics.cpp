#include <vector>
#include <random>     
#include <memory>
#include <iostream> 
#include "Constants.h"
#include "Enemy.h"
#include "Player.h" 

// Check if player is over the hole
bool isOverHole(float playerX) {
    return (playerX > HOLE_START_X && playerX < HOLE_END_X);
}

// Update player vertical position (falling logic)
float updateVerticalPosition(float playerY, bool& isFalling, bool& inTunnel, 
                           bool overHole, float dt) {
    // On platform
    if (!inTunnel && !overHole && !isFalling) {
        return TERRAIN_Y - PLAYER_SIZE;
    }
    else if (!inTunnel && (overHole || isFalling)) {
        isFalling = true;
        playerY += FALL_SPEED * dt;
        if (playerY >= TUNNEL_ENTRY_Y) {
            inTunnel = true;
        }
        return playerY;
    }
    else if (inTunnel) {
        playerY += FALL_SPEED * dt;
        return playerY;
    }
    return playerY;
}

// Constrain player horizontally inside tunnel
float constrainToTunnel(float playerX, bool inTunnel) {
    if (inTunnel) {
        if (playerX < TUNNEL_WALL_WIDTH) playerX = TUNNEL_WALL_WIDTH;
        if (playerX > WINDOW_WIDTH - TUNNEL_WALL_WIDTH - PLAYER_SIZE - 25.f) 
            playerX = WINDOW_WIDTH - TUNNEL_WALL_WIDTH - PLAYER_SIZE - 25.f;
    } else {
        if (playerX < 0.f)
            playerX = 0.f;

        if (playerX > WINDOW_WIDTH - PLAYER_SIZE)
            playerX = WINDOW_WIDTH - PLAYER_SIZE;
    }
    return playerX;
}

// Reset player to start position
void resetPlayer(bool& isFalling, bool& inTunnel) {
    isFalling = false;
    inTunnel = false;
}

void spawnWorm(std::vector<std::unique_ptr<Enemy>>& enemies, float playerY, float tunnelEntryY) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> chanceDist(0.0f, 1.0f);
    static std::uniform_int_distribution<> directionDist(0, 1);
    
    constexpr float PLATFORM_SPACING = 220.f;
    constexpr float FIRST_PLATFORM_Y = PLATFORM_1_Y;  // 620.f
    
    static int frameCounter = 0;
    frameCounter++;
    if (frameCounter % 30 != 0) return;
    
    if (chanceDist(gen) > 0.7f) return;  // 70%
    
    // Calculate player's layer
    int playerLayer = 0;
    if (playerY > FIRST_PLATFORM_Y) {
        playerLayer = static_cast<int>((playerY - FIRST_PLATFORM_Y) / PLATFORM_SPACING);
    }
    
    int layerOffset = 1 + static_cast<int>(chanceDist(gen) * 3);  // 1, 2, or 3
    int targetLayer = playerLayer + layerOffset;
    if (targetLayer < 0) targetLayer = 0;
    
    float wormY = FIRST_PLATFORM_Y + (targetLayer * PLATFORM_SPACING);

    // Count worms on this layer (max 2)
    int wormsOnLayer = 0;
    float tolerance = 50.f;
    
    for (const auto& enemy : enemies) {
        if (enemy->isActive) {
            float enemyY = enemy->shape->getPosition().y;
            if (std::abs(enemyY - wormY) < tolerance) {
                wormsOnLayer++;
            }
        }
    }
    
    if (wormsOnLayer >= 2) {
        return;
    }
    
    // Random X
    float minX = TUNNEL_WALL_WIDTH + 50.f;
    float maxX = WINDOW_WIDTH - TUNNEL_WALL_WIDTH - WORM_WIDTH - 50.f;
    std::uniform_real_distribution<> xDist(minX, maxX);
    float wormX = xDist(gen);
    
    int direction = directionDist(gen) == 0 ? -1 : 1;
    
    auto worm = std::make_unique<Enemy>();
    worm->spawn(wormX, wormY, direction, Enemy::Type::WORM);
    enemies.push_back(std::move(worm));
}

void spawnBat(std::vector<std::unique_ptr<Enemy>>& enemies, float playerY) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> chanceDist(0.0f, 1.0f);
    static std::uniform_int_distribution<> directionDist(0, 1);
    
    constexpr float AIR_GAP_SPACING = 220.f;
    constexpr float FIRST_AIR_GAP_Y = 730.f;
    
    static int frameCounter = 0;
    frameCounter++;
    if (frameCounter % 40 != 0) return;  // Slightly slower than worms
    
    if (chanceDist(gen) > BAT_SPAWN_CHANCE) return;
    
    // Calculate which air gap layer the player is near
    int playerGapLayer = 0;
    if (playerY > FIRST_AIR_GAP_Y) {
        playerGapLayer = static_cast<int>((playerY - FIRST_AIR_GAP_Y) / AIR_GAP_SPACING);
    }
    
    // Spawn ahead: layers +2 to +4 (bats appear before player reaches them)
    int gapOffset = 2 + static_cast<int>(chanceDist(gen) * 3);  // 2, 3, or 4
    int targetGap = playerGapLayer + gapOffset;
    
    // Calculate Y: first air gap + (layer * spacing)
    float batY = FIRST_AIR_GAP_Y + (targetGap * AIR_GAP_SPACING);
    
    // Count bats on this air gap (max 2)
    int batsOnGap = 0;
    float tolerance = 50.f;
    
    for (const auto& enemy : enemies) {
        if (enemy->isActive && enemy->enemyType == Enemy::Type::BAT) {
            float enemyY = enemy->shape->getPosition().y;
            if (std::abs(enemyY - batY) < tolerance) {
                batsOnGap++;
            }
        }
    }
    
    if (batsOnGap >= 2) return;
    
    // Random X
    float minX = TUNNEL_WALL_WIDTH + 50.f;
    float maxX = WINDOW_WIDTH - TUNNEL_WALL_WIDTH - BAT_WIDTH - 50.f;
    std::uniform_real_distribution<> xDist(minX, maxX);
    float batX = xDist(gen);
    
    int direction = directionDist(gen) == 0 ? -1 : 1;
    
    // Create and spawn bat
    auto bat = std::make_unique<Enemy>();
    bat->spawn(batX, batY, direction, Enemy::Type::BAT);  // Pass Type::BAT
    enemies.push_back(std::move(bat));
}

bool checkEnemyCollisions(const Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) {
    for (auto& enemy : enemies) {
        if (enemy->isActive && player.checkCollision(enemy->getBounds())) {
            return true;
        }
    }
    return false;
}

void cleanupEnemies(std::vector<std::unique_ptr<Enemy>>& enemies, float viewY) {
    for (auto& enemy : enemies) {
        if (enemy->isOffScreen(viewY)) {
            enemy->isActive = false;
        }
    }
}
