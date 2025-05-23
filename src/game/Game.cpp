#include "Game.h"
#include "../renderer/VulkanRenderer.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"

Game::Game() : isRunning(true) {}

void Game::init() {
    // Initialize Vulkan and other game components
    renderer.init();
    player.init();
    // enemy.init(); // Removed because Enemy has no init() method
}

void Game::update() {
    // Update game logic, handle input, and update entities
    player.update();
    enemy.update();
}

void Game::render() {
    // Render the game scene
    renderer.beginFrame();
    player.render();
    enemy.render();
    renderer.endFrame();
}

void Game::run() {
    init();
    while (isRunning) {
        update();
        render();
    }
}

void Game::cleanup() {
    // Cleanup resources
    renderer.cleanup();
    player.cleanup();
    enemy.cleanup();
}