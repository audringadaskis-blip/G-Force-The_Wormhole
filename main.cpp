#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}, 32), "The Wormhole");
    sf::Clock clock;

    // Player
    sf::RectangleShape player({30, 30});
    player.setFillColor(sf::Color::Green);
    player.setPosition({100, 220});

    // Physics
    const float fallSpeed = 150.f;
    const float moveSpeed = 200.f;
    
    bool isFalling = false;
    bool inTunnel = false;

    // Level
    const float terrainY = 250.f;
    const float holeStartX = 350.f;
    const float holeEndX = 450.f;
    const float tunnelEntryY = 350.f;
    const float rungSpacing = 120.f;

    // Tunnel walls <- basically will never end (for now)
    sf::RectangleShape tunnelLeft({100, 100000});
    sf::RectangleShape tunnelRight({100, 100000});
    tunnelLeft.setFillColor(sf::Color(50, 50, 50));
    tunnelRight.setFillColor(sf::Color(50, 50, 50));
    tunnelLeft.setPosition({0, -50000});
    tunnelRight.setPosition({700, -50000});

    // Camera
    sf::View view(sf::FloatRect({0, 0}, {800, 600}));
    view.setCenter({400, 300});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            
            // R to reset (for now, kadangi endless tunelis)
            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::R) {
                    player.setPosition({100, 220});
                    isFalling = false;
                    inTunnel = false;
                    view.setCenter({400, 300});
                }
            }
        }

        float dt = clock.restart().asSeconds();
        sf::Vector2f pos = player.getPosition();

        // Input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) 
            pos.x -= moveSpeed * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) 
            pos.x += moveSpeed * dt;

        // Is it over the hole? lol
        bool overHole = (pos.x > holeStartX && pos.x < holeEndX);

        // On horizontal tetrain
        if (!inTunnel && !overHole && !isFalling) {
            pos.y = terrainY - 30;
        }
        // Into the hole we go
        else if (!inTunnel && (overHole || isFalling)) {
            isFalling = true;
            pos.y += fallSpeed * dt;

            if (pos.y >= tunnelEntryY) {
                inTunnel = true;
            }
        }
        // Slow fall inside tunnel <- never ending
        else if (inTunnel) {
            pos.y += fallSpeed * dt;

            if (pos.x < 100) pos.x = 100;
            if (pos.x > 670) pos.x = 670;
        }

        // Smooth continuous camera follow
        float targetViewY;
        if (inTunnel) {
            targetViewY = pos.y - 150;
        } else if (isFalling) {
            float fallProgress = (pos.y - terrainY) / (tunnelEntryY - terrainY);
            if (fallProgress > 1.0f) fallProgress = 1.0f;
            targetViewY = 300 + (pos.y - 150 - 300) * fallProgress;
        } else {
            targetViewY = 300;
        }

        float currentViewY = view.getCenter().y;
        float newViewY = currentViewY + (targetViewY - currentViewY) * 10.0f * dt;
        view.setCenter({400, newViewY});

        player.setPosition(pos);

        // Drawing
        window.clear(sf::Color::Black);
        window.setView(view);

        window.draw(tunnelLeft);
        window.draw(tunnelRight);

        sf::RectangleShape groundLeft({holeStartX, 20});
        sf::RectangleShape groundRight({800 - holeEndX, 20});
        groundLeft.setFillColor(sf::Color(100, 100, 100));
        groundRight.setFillColor(sf::Color(100, 100, 100));
        groundLeft.setPosition({0, terrainY});
        groundRight.setPosition({holeEndX, terrainY});
        window.draw(groundLeft);
        window.draw(groundRight);

        // Dynamind rungs (do not ask, me do not know)
        // Only draw rungs that are visible on screen
        if (inTunnel) {
            float viewTop = view.getCenter().y - 300;
            float viewBottom = view.getCenter().y + 300;
            
            // Calculate which rungs are visible
            int startRung = static_cast<int>((viewTop - tunnelEntryY) / rungSpacing) - 1;
            int endRung = static_cast<int>((viewBottom - tunnelEntryY) / rungSpacing) + 1;
            
            for (int i = startRung; i <= endRung; i++) {
                if (i >= 0) {  // Only draw rungs after tunnel entry
                    sf::RectangleShape rung({600, 3});
                    rung.setFillColor(sf::Color(70, 70, 70));
                    rung.setPosition({100.0f, tunnelEntryY + 50.0f + i * rungSpacing});
                    window.draw(rung);
                }
            }
        }

        window.draw(player);
        window.display();
    }
    return 0;
}