#pragma once

#include "../renderer/VulkanRenderer.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"

enum class GameState {
    MENU,
    PLAYING
};

class Game {
public:
    Game();
    void init();
    void update();
    void render();
    void run();
    void cleanup();

    GameState state = GameState::MENU; // Add this line

private:
    bool isRunning;
    VulkanRenderer renderer;
    Player player;
    Enemy enemy;
};