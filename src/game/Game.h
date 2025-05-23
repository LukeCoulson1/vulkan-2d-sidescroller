#pragma once

#include "../renderer/VulkanRenderer.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"

// Add SETTINGS to GameState
enum class GameState {
    MENU,
    SETTINGS,
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
    void onResize(int width, int height); // <-- Add this line

    GameState state = GameState::MENU;

private:
    bool isRunning;
    VulkanRenderer renderer;
    Player player;
    Enemy enemy;
};