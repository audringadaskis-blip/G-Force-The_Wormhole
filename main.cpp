#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <iostream> 
#include <memory> 
#include "Constants.h"
#include "Player.h"
#include "Enemy.h"

struct FallingPlatform;
extern std::vector<FallingPlatform> fallingPlatforms;
void generateFallingPlatforms(int count);
void resetFallingPlatforms(int count);
void drawTunnel(sf::RenderWindow& window);
void drawPlatform(sf::RenderWindow& window);
void drawRungs(sf::RenderWindow& window, const sf::View& view, bool inTunnel);
void drawFinalPlatform(sf::RenderWindow& window);
void updateCamera(sf::View& view, const sf::Vector2f& playerPos, 
                  bool inTunnel, bool isFalling, float dt, bool reachedFinalPlatform);
bool isOverHole(float playerX);
float updateVerticalPosition(float playerY, bool& isFalling, bool& inTunnel, 
                           bool overHole, float dt, bool& reachedFinalPlatform);
float constrainToTunnel(float playerX, bool inTunnel);
void resetPlayer(bool& isFalling, bool& inTunnel);
void drawEnemies(sf::RenderWindow& window, std::vector<std::unique_ptr<Enemy>>& enemies);
void spawnWorm(std::vector<std::unique_ptr<Enemy>>& enemies, float playerY, float tunnelEntryY);
void spawnBat(std::vector<std::unique_ptr<Enemy>>& enemies, float playerY);
bool checkEnemyCollisions(const Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);
void cleanupEnemies(std::vector<std::unique_ptr<Enemy>>& enemies, float viewY);
void drawFallingPlatforms(sf::RenderWindow& window, const sf::View& view, bool inTunnel);


bool checkFallingPlatformCollisions(float& playerY, float playerX, bool& isFalling, bool spaceJustPressed);


int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}, 32), "The Wormhole");
    sf::Clock clock;
    
    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    sf::View view(sf::FloatRect({0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT}));
    view.setCenter({400.f, 300.f});
    
    bool canBreak = false;
    bool isFalling = false;
    bool inTunnel = false;
    bool gameOver = false;
    bool spaceWasPressed = false;
    bool reachedFinalPlatform = false;

    // background
    sf::Texture backgroundTexture0;
    sf::Texture backgroundTexture1;
    sf::Texture backgroundTexture2;
    sf::Texture backgroundTexture3;
    sf::Texture backgroundTexture4;
    sf::Texture backgroundTexture5;
    sf::Texture backgroundTexture6;
    sf::Texture backgroundTexture7;
    sf::Texture backgroundTexture8;
    sf::Texture backgroundTexture9;

    if (!backgroundTexture0.loadFromFile("assets/layer0.png")){return -1;}
    if (!backgroundTexture1.loadFromFile("assets/layer1.png")){return -1;}
    if (!backgroundTexture2.loadFromFile("assets/layer2.png")){return -1;}
    if (!backgroundTexture3.loadFromFile("assets/layer3.png")){return -1;}
    if (!backgroundTexture4.loadFromFile("assets/layer4.png")){return -1;}
    if (!backgroundTexture5.loadFromFile("assets/layer5.png")){return -1;}
    if (!backgroundTexture6.loadFromFile("assets/layer6.png")){return -1;}
    if (!backgroundTexture7.loadFromFile("assets/layer7.png")){return -1;}
    if (!backgroundTexture8.loadFromFile("assets/layer8.png")){return -1;}
    if (!backgroundTexture9.loadFromFile("assets/layer9.png")){return -1;}

    sf::Sprite backgroundSprite0(backgroundTexture0);
    sf::Sprite backgroundSprite1(backgroundTexture1);
    sf::Sprite backgroundSprite2(backgroundTexture2);
    sf::Sprite backgroundSprite3(backgroundTexture3);
    sf::Sprite backgroundSprite4(backgroundTexture4);
    sf::Sprite backgroundSprite5(backgroundTexture5);
    sf::Sprite backgroundSprite6(backgroundTexture6);
    sf::Sprite backgroundSprite7(backgroundTexture7);
    sf::Sprite backgroundSprite8(backgroundTexture8);
    sf::Sprite backgroundSprite9(backgroundTexture9);

    backgroundSprite0.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite1.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite2.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite3.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite4.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite5.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite6.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite7.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite8.setPosition(sf::Vector2f(0.f, 0.f));
    backgroundSprite9.setPosition(sf::Vector2f(0.f, 0.f));

    sf::Vector2u textureSize = backgroundTexture0.getSize();

    float zoomFactor = 0.6f;

    int cropWidth = static_cast<int>(textureSize.x * zoomFactor);
    int cropHeight = static_cast<int>(textureSize.y * zoomFactor);

    int cropX = (textureSize.x - cropWidth) / 2; // Center the crop area horizontally
    int cropY = textureSize.y - cropHeight - 50;

    sf::IntRect cropRect(sf::Vector2i(cropX, cropY), sf::Vector2i(cropWidth, cropHeight));

    backgroundSprite0.setTextureRect(cropRect);
    backgroundSprite1.setTextureRect(cropRect);
    backgroundSprite2.setTextureRect(cropRect);
    backgroundSprite3.setTextureRect(cropRect);
    backgroundSprite4.setTextureRect(cropRect);
    backgroundSprite5.setTextureRect(cropRect);
    backgroundSprite6.setTextureRect(cropRect);
    backgroundSprite7.setTextureRect(cropRect);
    backgroundSprite8.setTextureRect(cropRect);
    backgroundSprite9.setTextureRect(cropRect);

    float scaleX = 800.f / cropWidth;
    float scaleY = TERRAIN_Y / cropHeight;

    backgroundSprite0.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite1.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite2.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite3.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite4.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite5.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite6.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite7.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite8.setScale(sf::Vector2f(scaleX, scaleY));
    backgroundSprite9.setScale(sf::Vector2f(scaleX, scaleY));

    // background for tunnel
    sf::Texture caveTexture;
    if (!caveTexture.loadFromFile("assets/background1.png")) { return -1; }
    caveTexture.setRepeated(true);

    sf::Sprite caveSprite(caveTexture);
    caveSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(477, 100000)));

    float caveScaleX = 600.f / 477.f;
    float caveScaleY = caveScaleX;

    int caveTexHeight = static_cast<int>(TUNNEL_HEIGHT / caveScaleY);
    caveSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(477, caveTexHeight)));
    
    caveSprite.setScale(sf::Vector2f(caveScaleX, caveScaleY));
    caveSprite.setPosition(sf::Vector2f(TUNNEL_LEFT_X + TUNNEL_WALL_WIDTH, TUNNEL_OFFSET));

    // background for tunnel walls

    sf::Texture wallTexture;
    if (!wallTexture.loadFromFile("assets/dirt2.png")) { return -1; }
    wallTexture.setRepeated(true);

    sf::Sprite leftWallSprite(wallTexture);
    sf::Sprite rightWallSprite(wallTexture);

    int wallHeight = static_cast<int>(TUNNEL_HEIGHT);

    // Cave background
    caveSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(477, wallHeight)));

    // Left and right walls
    leftWallSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(100, wallHeight)));
    rightWallSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(100, wallHeight)));

    float wallScaleX = 1.f;  // no scaling at all, 100px texture = 100px wall
    float wallScaleY = 1.f;

    leftWallSprite.setScale(sf::Vector2f(wallScaleX, wallScaleY));
    rightWallSprite.setScale(sf::Vector2f(wallScaleX, wallScaleY));

    leftWallSprite.setPosition(sf::Vector2f(TUNNEL_LEFT_X, TUNNEL_OFFSET));
    rightWallSprite.setPosition(sf::Vector2f(TUNNEL_RIGHT_X, TUNNEL_OFFSET));

    //sky background

    sf::Texture skyTexture;
    if (!skyTexture.loadFromFile("assets/falling.png")) { return -1; } // Make sure to use your actual file name!
    
    sf::Sprite skySprite(skyTexture);

    float skyPixelHeight = FINAL_PLATFORM_Y - SKY_START_Y;
    
    float skyWidth = WINDOW_WIDTH; 

    sf::Vector2u skySize = skyTexture.getSize();
    float skyScaleX = skyWidth / skySize.x;
    float skyScaleY = skyPixelHeight / skySize.y;

    skySprite.setScale(sf::Vector2f(skyScaleX, skyScaleY));
    
    skySprite.setPosition(sf::Vector2f(0.f, SKY_START_Y));

    //Beach background

    sf::Texture bottomTexture1, bottomTexture2, bottomTexture3, bottomTexture4, bottomTexture5;
    if (!bottomTexture1.loadFromFile("assets/ocean1.png")) { return -1; }
    if (!bottomTexture2.loadFromFile("assets/ocean2.png")) { return -1; }
    if (!bottomTexture3.loadFromFile("assets/ocean3.png")) { return -1; }
    if (!bottomTexture4.loadFromFile("assets/ocean4.png")) { return -1; }
    if (!bottomTexture5.loadFromFile("assets/ocean5.png")) { return -1; }

    sf::Sprite bottomSprite1(bottomTexture1);
    sf::Sprite bottomSprite2(bottomTexture2);
    sf::Sprite bottomSprite3(bottomTexture3);
    sf::Sprite bottomSprite4(bottomTexture4);
    sf::Sprite bottomSprite5(bottomTexture5);

    auto setupBottomSprite = [](sf::Sprite& sprite, const sf::Texture& tex, float yOffsetFromFinalPlatform) {
        float scaleX = static_cast<float>(WINDOW_WIDTH) / tex.getSize().x;
        float scaleY = scaleX;
        sprite.setScale(sf::Vector2f(scaleX, scaleY));
        
        float spriteHeight = tex.getSize().y * scaleY;
        sprite.setPosition(sf::Vector2f(0.f, FINAL_PLATFORM_Y - spriteHeight + yOffsetFromFinalPlatform));
    };

    setupBottomSprite(bottomSprite3, bottomTexture3, 0.f);
    setupBottomSprite(bottomSprite4, bottomTexture4, 0.f);
    setupBottomSprite(bottomSprite1, bottomTexture1, 0.f);
    setupBottomSprite(bottomSprite2, bottomTexture2, 0.f);
    setupBottomSprite(bottomSprite5, bottomTexture5, 0.f);

    generateFallingPlatforms(20);

    static bool victorySetupDone = false;
    static std::vector<sf::Sprite> victoryClones;
    static sf::Texture victoryTexture;
    static sf::Sprite victorySprite(wallTexture); 
    static bool victoryAssetsLoaded = false;
    static sf::Clock victoryInputClock;
    
    while (window.isOpen()) {
        // Events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (auto* key = event->getIf<sf::Event::KeyPressed>())
                if (key->code == sf::Keyboard::Key::R) {
                    resetPlayer(isFalling, inTunnel);
                    reachedFinalPlatform = false;
                    player.setPosition(PLAYER_START_X, PLAYER_START_Y);
                    enemies.clear();
                    gameOver = false;
                    view.setCenter({400.f, 300.f});

                    victorySetupDone = false;
                    victoryClones.clear();
                }
        }

        float dt = clock.restart().asSeconds();

        float prevX = player.getPosition().x;
        float prevY = player.getPosition().y;
       

        // Input
        bool moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || 
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
        bool moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || 
                sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
       
        bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
        bool spaceJustPressed = spacePressed && !spaceWasPressed;
        spaceWasPressed = spacePressed;
        player.update(dt, moveLeft, moveRight);
        sf::Vector2f pos = player.getPosition();
        bool overHole = isOverHole(pos.x);

        // Game logic
       
        pos.y = updateVerticalPosition(pos.y, isFalling, inTunnel, overHole, dt, reachedFinalPlatform);        
        canBreak = checkFallingPlatformCollisions(pos.y, pos.x, isFalling, spaceJustPressed);
        pos.x = constrainToTunnel(pos.x, inTunnel);
        player.setPosition(pos.x, pos.y);
        

        
        if (inTunnel) {
            spawnWorm(enemies, pos.y, TUNNEL_ENTRY_Y);
            spawnBat(enemies, pos.y); 
        }

        for (auto& enemy : enemies) {
            enemy->update(dt);
        }
        
        if (checkEnemyCollisions(player, enemies)) {
            std::cout << "Hit by worm! Restarting...\n"; // for now just auto-restarting, to get rid of the continuous "Game over! Hit by worm" or smth as such
            
            resetPlayer(isFalling, inTunnel);
            player.setPosition(PLAYER_START_X, PLAYER_START_Y);
            enemies.clear();

            resetFallingPlatforms(20);
            view.setCenter({400.f, 300.f});

            victorySetupDone = false;
            victoryClones.clear();
        }
            
        cleanupEnemies(enemies, view.getCenter().y);

        // Camera
        updateCamera(view, pos, inTunnel, isFalling, dt, reachedFinalPlatform);

        // Render
        window.clear(COLOR_BACKGROUND);
        window.setView(view);

        window.draw(backgroundSprite0);
        window.draw(backgroundSprite1);
        window.draw(backgroundSprite2);
        window.draw(backgroundSprite3);
        window.draw(backgroundSprite4);
        window.draw(backgroundSprite5);
        window.draw(backgroundSprite6);
        window.draw(backgroundSprite7);
        window.draw(backgroundSprite8);
        window.draw(backgroundSprite9);

        window.draw(caveSprite);
        window.draw(leftWallSprite);
        window.draw(rightWallSprite);

        window.draw(skySprite); 

        window.draw(bottomSprite3);
        window.draw(bottomSprite4); 
        window.draw(bottomSprite1);
        window.draw(bottomSprite2);
        window.draw(bottomSprite5);

        drawPlatform(window);
        // drawRungs(window, view, inTunnel);
        drawFallingPlatforms(window, view, inTunnel);
        drawFinalPlatform(window); 
        drawEnemies(window, enemies); 
        player.draw(window);

        if (reachedFinalPlatform) {
            if (!victorySetupDone) {
                // Užkrauname tikrąjį pergalės užrašą
                if (victoryTexture.loadFromFile("assets/victory.png")) { 
                    victoryAssetsLoaded = true;
                    victorySprite.setTexture(victoryTexture);
                    victorySprite.setTextureRect(sf::IntRect({0, 0}, static_cast<sf::Vector2i>(victoryTexture.getSize())));
                    sf::Vector2u vSize = victoryTexture.getSize();
                    victorySprite.setOrigin({vSize.x / 2.f, vSize.y / 2.f});
                }

                for (int i = 0; i < 50; ++i) {
                    sf::Sprite clone(player.texture);
                    if (player.sprite) {
                        clone.setTextureRect(player.sprite->getTextureRect());
                        clone.setScale(player.sprite->getScale());
                    }
                    
                    float cloneX = 50.f + (rand() % 700);
                    float cloneY = WINDOW_HEIGHT - PLAYER_SIZE - 150.f;
                    
                    clone.setPosition({cloneX, cloneY});
                    victoryClones.push_back(clone);
                }
                victorySetupDone = true;
                victoryInputClock.restart(); 
            }

            window.setView(window.getDefaultView());

            for (const auto& clone : victoryClones) {
                window.draw(clone);
            }

            if (reachedFinalPlatform && victoryAssetsLoaded) {
                victorySprite.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
                window.draw(victorySprite);
            }

            window.setView(view);

            if (victoryAssetsLoaded) {
                window.setView(window.getDefaultView()); 
                victorySprite.setPosition({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
                window.draw(victorySprite);
                window.setView(view); 
            }

            // Išjungimas po 0.4s apsaugos paspaudus Space, Enter arba Escape
            if (reachedFinalPlatform && victoryInputClock.getElapsedTime().asSeconds() > 0.4f) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    window.close();
                }
            }
        }
        window.display();
    }
    return 0;
}
