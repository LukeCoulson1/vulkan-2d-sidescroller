#pragma once
#include "Button.h"
#include <vector>
#include <memory>

class Menu {
public:
    std::vector<std::unique_ptr<Button>> buttons;

    void addButton(float x, float y, float w, float h, GLuint texture, const std::string& label) {
        buttons.push_back(std::make_unique<Button>(x, y, w, h, texture, label));
    }

    void updateHover(double mx, double my) {
        for (auto& btn : buttons)
            btn->hovered = btn->isMouseOver(mx, my);
    }

    void draw() const {
        for (const auto& btn : buttons)
            btn->draw();
    }

    int getClicked(double mx, double my, int mousePressed) const {
        if (mousePressed) {
            for (size_t i = 0; i < buttons.size(); ++i)
                if (buttons[i]->isMouseOver(mx, my))
                    return static_cast<int>(i);
        }
        return -1;
    }
};