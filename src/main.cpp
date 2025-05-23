#include <GLFW/glfw3.h>
#include "game/Game.h"
#include <iostream>

void drawMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    // Simple text placeholder (real text rendering needs more setup)
    // For now, just clear the screen to a color
    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    // You can add OpenGL code to draw a rectangle or logo here if you want
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan 2D Side Scroller", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Set up 2D projection for OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    Game game;
    game.init();

    // Main menu loop
    while (game.state == GameState::MENU && !glfwWindowShouldClose(window)) {
        drawMenu();
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            game.state = GameState::PLAYING;
        }
    }

    // Main game loop
    while (game.state == GameState::PLAYING && !glfwWindowShouldClose(window)) {
        game.update();
        game.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    game.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}