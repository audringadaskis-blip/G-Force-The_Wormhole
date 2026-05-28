#include <vector>
#include <random>     
#include <memory>
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
    
    if (chanceDist(gen) > WORM_SPAWN_CHANCE) return;
    
    float spawnY = playerY + WORM_SPAWN_DISTANCE;
    int rungIndex = static_cast<int>((spawnY - tunnelEntryY) / RUNG_SPACING);
    float wormY = tunnelEntryY + 50.f + rungIndex * RUNG_SPACING;
    
    int wormsOnRung = 0;
    for (auto& enemy : enemies) {
        if (enemy->isActive) {
            int enemyRungIndex = static_cast<int>((enemy->shape->getPosition().y - tunnelEntryY) / RUNG_SPACING);
            if (enemyRungIndex == rungIndex) {
                wormsOnRung++;
            }
        }
    }

    if (wormsOnRung >= 2) {
        return;
    }
    
    // Random X within tunnel
    float minX = TUNNEL_WALL_WIDTH + 50.f;
    float maxX = WINDOW_WIDTH - TUNNEL_WALL_WIDTH - WORM_WIDTH - 50.f;
    std::uniform_real_distribution<> xDist(minX, maxX);
    float wormX = xDist(gen);
    
    int direction = directionDist(gen) == 0 ? -1 : 1;
    
    // Create new enemy
    auto newEnemy = std::make_unique<Enemy>();
    newEnemy->spawn(wormX, wormY, direction);
    enemies.push_back(std::move(newEnemy));
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
