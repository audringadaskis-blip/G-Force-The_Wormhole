#pragma once
#include <SFML/Graphics.hpp>

// Window
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// Player
constexpr float PLAYER_SIZE = 30.f;
constexpr float PLAYER_START_X = 100.f;
constexpr float PLAYER_START_Y = 220.f;
constexpr float SPRITE_OFFSET_Y = -50.f;
constexpr const char* PLAYER_TEXTURE_PATH = "assets/character.png";

// Enemies/Worms
constexpr float WORM_WIDTH = 60.f;
constexpr float WORM_HEIGHT = 50.f;
constexpr float WORM_SPEED = 80.f;
constexpr float WORM_SPAWN_DISTANCE = 250.f;
constexpr float WORM_SPAWN_CHANCE = 0.3f;
constexpr int WORM_COLOR_R = 180;
constexpr int WORM_COLOR_G = 50;
constexpr int WORM_COLOR_B = 50;
constexpr float WORM_ANIMATION_SPEED = 0.15f;
constexpr const char* WORM_RIGHT_1 = "assets/worm_r_1.png";
constexpr const char* WORM_RIGHT_2 = "assets/worm_r_2.png";
constexpr const char* WORM_LEFT_1 = "assets/worm_l_1.png";
constexpr const char* WORM_LEFT_2 = "assets/worm_l_2.png";

// Enemies/Bats
constexpr float BAT_WIDTH = 60.f;
constexpr float BAT_HEIGHT = 40.f;
constexpr float BAT_SPEED = 100.f;
constexpr float BAT_ANIMATION_SPEED = 0.12f;
constexpr float BAT_SPAWN_CHANCE = 0.4f;

// Bat textures
constexpr const char* BAT_RIGHT_1 = "assets/1.png";
constexpr const char* BAT_RIGHT_2 = "assets/2.png";
constexpr const char* BAT_RIGHT_3 = "assets/3.png";
constexpr const char* BAT_LEFT_1 = "assets/4.png";
constexpr const char* BAT_LEFT_2 = "assets/5.png";
constexpr const char* BAT_LEFT_3 = "assets/6.png";

// for worms
constexpr float PLATFORM_1_Y = 425.f;   // Top platform (moved down) -220.f, platformos yra kas 220.f
constexpr float PLATFORM_2_Y = 840.f;   // Middle platform
constexpr float PLATFORM_3_Y = 1060.f;   // Bottom platform

// for bats
constexpr float AIR_GAP_1_Y = 535.f;   // Between platform 1 (620) and 2 (840)
constexpr float AIR_GAP_2_Y = 950.f;   // Between platform 2 (840) and 3 (1060)
constexpr float AIR_GAP_SPACING = 220.f;  // Same as platform spacing
constexpr float FIRST_AIR_GAP_Y = 730.f;  // First air gap Y position

// Physics
constexpr float FALL_SPEED = 150.f;
constexpr float MOVE_SPEED = 200.f;
constexpr float CAMERA_SMOOTHNESS = 8.f;
constexpr float TUNNEL_TRANSITION_SPEED = 3.f;

// Level
constexpr float TERRAIN_Y = 250.f;
constexpr float HOLE_START_X = 350.f;
constexpr float HOLE_END_X = 450.f;
constexpr float TUNNEL_ENTRY_Y = 350.f;
constexpr float RUNG_SPACING = 120.f;

// Tunnel
constexpr float TUNNEL_WALL_WIDTH = 100.f;
constexpr float TUNNEL_LEFT_X = 0.f;
constexpr float TUNNEL_RIGHT_X = 700.f;
constexpr float TUNNEL_HEIGHT = 100000.f;
constexpr float TUNNEL_OFFSET = TERRAIN_Y;

// Colors
constexpr sf::Color COLOR_BACKGROUND = sf::Color::Black;
constexpr sf::Color COLOR_TUNNEL_WALL(50, 50, 50);
constexpr sf::Color COLOR_PLATFORM(100, 100, 100);
constexpr sf::Color COLOR_RUNG(70, 70, 70);