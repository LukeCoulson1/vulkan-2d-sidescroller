#pragma once
#include "Button.h"
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include "game/Game.h"

class SettingsMenu {
public:
    Button backButton;
    // Dropdown state
    bool dropdownOpen = false;
    int selectedResolution = 0;
    int hoverOption = -1;
    // Checkbox state
    bool fullscreen = false;
    bool hoverDrop = false;
    bool hoverCB = false;

    SettingsMenu(GLuint backTex, int virtualWidth, int virtualHeight)
        : backButton((virtualWidth - 200) / 2.0f, virtualHeight * 0.15f, 200, 50, backTex, "Back") {}

    void updateHover(double mx, double my) {
        backButton.hovered = backButton.isMouseOver(mx, my);
        // ...dropdown and checkbox hover logic here...
    }

    void draw() const {
        backButton.draw();
        // ...dropdown and checkbox drawing here...
    }

    bool isBackClicked(double mx, double my, int mousePressed) const {
        return mousePressed && backButton.isMouseOver(mx, my);
    }
    // ...dropdown and checkbox click logic here...

    void update(double mx, double my, bool mousePressed) {
        updateHover(mx, my);
        // ...update logic for dropdown and checkbox states...
    }

    void handleClick(GLFWwindow* window, Game& game, std::vector<std::pair<int, int>>& resolutions,
                     double mx, double my, int mousePressed) {
        if (isBackClicked(mx, my, mousePressed)) {
            // ...handle back button click...
        }
        // ...handle dropdown and checkbox clicks...
    }
};