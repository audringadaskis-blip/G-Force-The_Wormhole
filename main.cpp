#include <SFML/Graphics.hpp>
#include <optional>
#include "Constants.h"
#include "Player.h"

void drawTunnel(sf::RenderWindow& window);
void drawPlatform(sf::RenderWindow& window);
void drawRungs(sf::RenderWindow& window, const sf::View& view, bool inTunnel);
void updateCamera(sf::View& view, const sf::Vector2f& playerPos, 
                  bool inTunnel, bool isFalling, float dt);

bool isOverHole(float playerX);
float updateVerticalPosition(float playerY, bool& isFalling, bool& inTunnel, 
                           bool overHole, float dt);
float constrainToTunnel(float playerX, bool inTunnel);
void resetPlayer(bool& isFalling, bool& inTunnel);

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}, 32), "The Wormhole");
    sf::Clock clock;
    
    Player player;
    sf::View view(sf::FloatRect({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT}));
    view.setCenter({400.f, 300.f});
    
    bool isFalling = false;
    bool inTunnel = false;

    while (window.isOpen()) {
        // Events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::R) {
                    resetPlayer(isFalling, inTunnel);
                    player.setPosition(PLAYER_START_X, PLAYER_START_Y);
                }
            }
        }

        float dt = clock.restart().asSeconds();
        sf::Vector2f pos = player.getPosition();

        // Input
        bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
        bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
        player.update(dt, moveLeft, moveRight);
        pos = player.getPosition();

        // Game logic
        bool overHole = isOverHole(pos.x);
        pos.y = updateVerticalPosition(pos.y, isFalling, inTunnel, overHole, dt);
        pos.x = constrainToTunnel(pos.x, inTunnel);
        player.setPosition(pos.x, pos.y);

        // Camera
        updateCamera(view, pos, inTunnel, isFalling, dt);

        // Render
        window.clear(COLOR_BACKGROUND);
        window.setView(view);
        
        drawTunnel(window);
        drawPlatform(window);
        drawRungs(window, view, inTunnel);
        player.draw(window);
        
        window.display();
    }
    return 0;
}