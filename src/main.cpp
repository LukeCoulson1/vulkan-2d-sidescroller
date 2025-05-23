#include <GLFW/glfw3.h>
#include "game/Game.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Button struct for convenience
struct Button {
    float x, y, w, h;
    const char* label;
    GLuint texture;
};

Button newGameBtn = {300, 350, 200, 60, "New Game", 0};
Button settingsBtn = {300, 250, 200, 60, "Settings", 0};

// Load an image as an OpenGL texture
GLuint loadTexture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return 0;
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return tex;
}

bool isMouseOver(const Button& btn, double mx, double my) {
    return mx >= btn.x && mx <= btn.x + btn.w && my >= btn.y && my <= btn.y + btn.h;
}

void drawButton(const Button& btn, bool hovered) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, btn.texture);
    glColor3f(hovered ? 0.8f : 1.0f, hovered ? 0.8f : 1.0f, hovered ? 0.8f : 1.0f); // Optional highlight

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(btn.x, btn.y);
        glTexCoord2f(1, 0); glVertex2f(btn.x + btn.w, btn.y);
        glTexCoord2f(1, 1); glVertex2f(btn.x + btn.w, btn.y + btn.h);
        glTexCoord2f(0, 1); glVertex2f(btn.x, btn.y + btn.h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawMenu(bool hoverNew, bool hoverSettings) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

    drawButton(newGameBtn, hoverNew);
    drawButton(settingsBtn, hoverSettings);
}

int main() {
    if (!glfwInit()) return -1;

    // Flip images vertically on load so they appear correctly in OpenGL
    stbi_set_flip_vertically_on_load(1);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan 2D Side Scroller", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // Load button textures (corrected paths for your setup)
    newGameBtn.texture = loadTexture("../../img/New_Game.png");
    settingsBtn.texture = loadTexture("../../img/Settings.png");

    Game game;
    game.init();

    // Main menu loop
    while (game.state == GameState::MENU && !glfwWindowShouldClose(window)) {
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        my = 600 - my; // Flip Y for OpenGL coordinates

        bool hoverNew = isMouseOver(newGameBtn, mx, my);
        bool hoverSettings = isMouseOver(settingsBtn, mx, my);

        drawMenu(hoverNew, hoverSettings);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (hoverNew) {
                game.state = GameState::PLAYING;
            }
            if (hoverSettings) {
                std::cout << "Settings button clicked!\n";
                // You can add settings logic here
            }
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