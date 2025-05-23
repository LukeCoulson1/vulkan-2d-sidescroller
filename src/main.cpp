#include <GL/glew.h>    // Always include GLEW first!
#include <GLFW/glfw3.h>
#include <iostream>
#include "game/Game.h"
#include "ui/Menu.h"
#include "ui/SettingsMenu.h"
#include "ui/ResourceManager.h"
#include <stb_image.h>

int main() {
    if (!glfwInit()) return -1;

    stbi_set_flip_vertically_on_load(1);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan 2D Side Scroller", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLUT for bitmap font rendering
    // GLUT initialization removed (using GLFW for window/context management)
    // Load button textures
    GLuint newGameTex = ResourceManager::loadTexture("../../img/New_Game.png");
    GLuint settingsTex = ResourceManager::loadTexture("../../img/Settings.png");
    GLuint backTex = ResourceManager::loadTexture("../../img/New_Game.png"); // Placeholder

    GameState state = GameState::MENU;
    Game game;
    game.init();

    const int virtualWidth = 800;
    const int virtualHeight = 600;

    // Menu object
    Menu mainMenu;
    mainMenu.addButton((virtualWidth - 200) / 2.0f, 360, 200, 60, newGameTex, "New Game");
    mainMenu.addButton((virtualWidth - 200) / 2.0f, 240, 200, 60, settingsTex, "Settings");

    // Settings menu object
    SettingsMenu settingsMenu(backTex, virtualWidth, virtualHeight);

    // Example resolutions vector (pass to settingsMenu as needed)
    std::vector<std::pair<int, int>> resolutions = {
        {800, 600},
        {1024, 768},
        {1280, 720},
        {1920, 1080}
    };

    while (!glfwWindowShouldClose(window)) {
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        // Set up orthographic projection for virtual resolution
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, virtualWidth, 0, virtualHeight, -1, 1);
        glMatrixMode(GL_MODELVIEW);

        int winWidth, winHeight;
        glfwGetWindowSize(window, &winWidth, &winHeight);

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        // Robust DPI-safe mouse conversion
        if (winWidth == 0) winWidth = 1;
        if (winHeight == 0) winHeight = 1;
        double normX = mx / winWidth;
        double normY = my / winHeight;

        // Convert to virtual coordinates
        double mx_virtual = normX * virtualWidth;
        double my_virtual = (1.0 - normY) * virtualHeight;

        // Mouse button state (single frame detection)
        static bool mousePressed = false;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            mousePressed = true;
        } else {
            mousePressed = false;
        }

        if (state == GameState::MENU) {
            mainMenu.updateHover(mx_virtual, my_virtual);
            mainMenu.draw();

            int clicked = mainMenu.getClicked(mx_virtual, my_virtual, mousePressed);
            if (clicked == 0) state = GameState::PLAYING;
            if (clicked == 1) state = GameState::SETTINGS;

            glfwSwapBuffers(window);
            glfwPollEvents();
        } else if (state == GameState::SETTINGS) {
            settingsMenu.update(mx_virtual, my_virtual, mousePressed);
            settingsMenu.draw();
            settingsMenu.handleClick(window, game, resolutions, mx_virtual, my_virtual, mousePressed);
            if (settingsMenu.isBackClicked(mx_virtual, my_virtual, mousePressed)) {
                state = GameState::MENU;
            }
            glfwSwapBuffers(window);
            glfwPollEvents();
        } else if (state == GameState::PLAYING) {
            game.update();
            game.render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    game.cleanup();
    ResourceManager::clear();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}