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
// LEFT is right and RIGHT is left..:D
constexpr const char* WORM_RIGHT_1 = "assets/worm_l_1.png";
constexpr const char* WORM_RIGHT_2 = "assets/worm_l_2.png";
constexpr const char* WORM_LEFT_1 = "assets/worm_r_1.png";
constexpr const char* WORM_LEFT_2 = "assets/worm_r_2.png";

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