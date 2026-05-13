#include "Constants.h"

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
void constrainToTunnel(float playerX, bool inTunnel) {
    if (inTunnel) {
        if (playerX < TUNNEL_WALL_WIDTH) playerX = TUNNEL_WALL_WIDTH;
        if (playerX > WINDOW_WIDTH - TUNNEL_WALL_WIDTH - PLAYER_SIZE) 
            playerX = WINDOW_WIDTH - TUNNEL_WALL_WIDTH - PLAYER_SIZE;
    }
}

// Reset player to start position
void resetPlayer(bool& isFalling, bool& inTunnel, sf::View& view) {
    isFalling = false;
    inTunnel = false;
    view.setCenter({400.f, 300.f});
}