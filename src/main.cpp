#include <GLFW/glfw3.h>
#include "game/Game.h"
#include <iostream>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/freeglut.h>

// Settings UI state
struct SettingsState {
    int selectedResolution = 0;
    bool fullscreen = false;
    bool dropdownOpen = false;
};

std::vector<std::pair<int, int>> resolutions = {
    {800, 600},
    {1024, 768},
    {1280, 720},
    {1920, 1080}
};
std::vector<std::string> resolutionLabels = {
    "800 x 600",
    "1024 x 768",
    "1280 x 720",
    "1920 x 1080"
};

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

bool isMouseOver(float x, float y, float w, float h, double mx, double my) {
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}

void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

void drawButton(float x, float y, float w, float h, GLuint texture, bool hovered) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(hovered ? 0.8f : 1.0f, hovered ? 0.8f : 1.0f, hovered ? 0.8f : 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + w, y);
        glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
        glTexCoord2f(0, 1); glVertex2f(x, y + h);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void updateOrtho(GLFWwindow* window) {
    // Use a fixed virtual resolution
    int virtualWidth = 800;
    int virtualHeight = 600;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, virtualWidth, 0, virtualHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void drawMenu(int virtualWidth, int virtualHeight, GLuint newGameTex, GLuint settingsTex, double mx, double my, bool& hoverNew, bool& hoverSettings) {
    // Use fixed sizes and positions in virtual space
    float btnW = 200;
    float btnH = 60;
    float btnX = (virtualWidth - btnW) / 2.0f; // Centered
    float newGameY = 360;
    float settingsY = 240;

    hoverNew = isMouseOver(btnX, newGameY, btnW, btnH, mx, my);
    hoverSettings = isMouseOver(btnX, settingsY, btnW, btnH, mx, my);

    drawButton(btnX, newGameY, btnW, btnH, newGameTex, hoverNew);
    drawButton(btnX, settingsY, btnW, btnH, settingsTex, hoverSettings);
}

void drawSettingsMenu(SettingsState& settings, int virtualWidth, int virtualHeight, GLuint backTex, double mx, double my, bool& hoverBack, bool& hoverDrop, bool& hoverCB, int& hoverOption) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.15f, 0.15f, 0.2f, 1.0f);

    // Dropdown
    float dropW = 200.0f;
    float dropH = 40.0f;
    float dropX = (virtualWidth - dropW) / 2.0f;
    float dropY = virtualHeight * 0.58f;
    hoverDrop = isMouseOver(dropX, dropY, dropW, dropH, mx, my);
    drawRect(dropX, dropY, dropW, dropH, hoverDrop ? 0.3f : 0.2f, 0.5f, 0.7f);

    // Draw selected resolution text
    glColor3f(1, 1, 1);
    glRasterPos2f(dropX + 10, dropY + dropH / 2);
    for (char c : resolutionLabels[settings.selectedResolution]) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Dropdown options
    hoverOption = -1;
    if (settings.dropdownOpen) {
        for (int i = 0; i < (int)resolutions.size(); ++i) {
            float optY = dropY - (i + 1) * dropH;
            bool hoverOpt = isMouseOver(dropX, optY, dropW, dropH, mx, my);
            if (hoverOpt) hoverOption = i;
            drawRect(dropX, optY, dropW, dropH, hoverOpt ? 0.4f : 0.25f, 0.6f, 0.8f);
            glColor3f(1, 1, 1);
            glRasterPos2f(dropX + 10, optY + dropH / 2);
            for (char c : resolutionLabels[i]) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
    }

    // Fullscreen checkbox
    float cbS = dropH;
    float cbX = dropX;
    float cbY = dropY - dropH * 2;
    hoverCB = isMouseOver(cbX, cbY, cbS, cbS, mx, my);
    drawRect(cbX, cbY, cbS, cbS, hoverCB ? 0.4f : 0.2f, 0.7f, 0.4f);
    if (settings.fullscreen) {
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);
            glVertex2f(cbX, cbY);
            glVertex2f(cbX + cbS, cbY + cbS);
            glVertex2f(cbX + cbS, cbY);
            glVertex2f(cbX, cbY + cbS);
        glEnd();
    }
    // Label for checkbox
    glColor3f(1, 1, 1);
    glRasterPos2f(cbX + cbS + 10, cbY + cbS / 2);
    std::string fsLabel = "Fullscreen";
    for (char c : fsLabel) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    // Back button (bottom center)
    float btnW = 200.0f;
    float btnH = 50.0f;
    float btnX = (virtualWidth - btnW) / 2.0f;
    float backY = virtualHeight * 0.15f;
    hoverBack = isMouseOver(btnX, backY, btnW, btnH, mx, my);
    drawButton(btnX, backY, btnW, btnH, backTex, hoverBack);
}

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
    int argc = 1;
    char* argv[1] = { (char*)"Something" };
    glutInit(&argc, argv);

    // Load button textures (corrected paths for your setup)
    GLuint newGameTex = loadTexture("../../img/New_Game.png");
    GLuint settingsTex = loadTexture("../../img/Settings.png");
    GLuint backTex = loadTexture("../../img/New_Game.png"); // Placeholder

    GameState state = GameState::MENU;
    SettingsState settings;

    Game game;
    game.init();

    const int virtualWidth = 800;
    const int virtualHeight = 600;

    while (!glfwWindowShouldClose(window)) {
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
        updateOrtho(window);

        int winWidth, winHeight;
        glfwGetWindowSize(window, &winWidth, &winHeight);

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        // --- Robust DPI-safe mouse conversion ---
        if (winWidth == 0) winWidth = 1;
        if (winHeight == 0) winHeight = 1;
        double normX = mx / winWidth;
        double normY = my / winHeight;

        // Convert to virtual coordinates
        double mx_virtual = normX * virtualWidth;
        double my_virtual = (1.0 - normY) * virtualHeight;

        if (state == GameState::MENU) {
            bool hoverNew = false, hoverSettings = false;
            drawMenu(virtualWidth, virtualHeight, newGameTex, settingsTex, mx_virtual, my_virtual, hoverNew, hoverSettings);
            glfwSwapBuffers(window);
            glfwPollEvents();

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (hoverNew) {
                    state = GameState::PLAYING;
                }
                if (hoverSettings) {
                    state = GameState::SETTINGS;
                }
            }
        } else if (state == GameState::SETTINGS) {
            bool hoverBack = false, hoverDrop = false, hoverCB = false;
            int hoverOption = -1;
            drawSettingsMenu(settings, virtualWidth, virtualHeight, backTex, mx_virtual, my_virtual, hoverBack, hoverDrop, hoverCB, hoverOption);
            glfwSwapBuffers(window);
            glfwPollEvents();

            // Dropdown logic (all in virtual coordinates)
            float dropW = 200.0f;
            float dropH = 40.0f;
            float dropX = (virtualWidth - dropW) / 2.0f;
            float dropY = virtualHeight * 0.58f;
            float cbS = dropH;
            float cbX = dropX;
            float cbY = dropY - dropH * 2;
            float btnW = 200.0f;
            float btnH = 50.0f;
            float btnX = (virtualWidth - btnW) / 2.0f;
            float backY = virtualHeight * 0.15f;

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (settings.dropdownOpen && hoverOption != -1) {
                    settings.selectedResolution = hoverOption;
                    settings.dropdownOpen = false;
                    // Change window size if not fullscreen
                    if (!settings.fullscreen) {
                        glfwSetWindowSize(window, resolutions[hoverOption].first, resolutions[hoverOption].second);
                        updateOrtho(window);
                        game.onResize(resolutions[hoverOption].first, resolutions[hoverOption].second);
                    }
                } else if (hoverDrop) {
                    settings.dropdownOpen = !settings.dropdownOpen;
                } else if (hoverCB) {
                    bool wasFullscreen = settings.fullscreen;
                    settings.fullscreen = !settings.fullscreen;
                    if (settings.fullscreen) {
                        // Enter fullscreen
                        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                        updateOrtho(window);
                        game.onResize(mode->width, mode->height);
                    } else if (wasFullscreen) {
                        // Leave fullscreen: restore windowed mode at selected resolution
                        auto res = resolutions[settings.selectedResolution];
                        glfwSetWindowMonitor(window, nullptr, 100, 100, res.first, res.second, 0);
                        updateOrtho(window);
                        game.onResize(res.first, res.second);
                    }
                } else if (hoverBack) {
                    state = GameState::MENU;
                } else {
                    settings.dropdownOpen = false;
                }
            }
        } else if (state == GameState::PLAYING) {
            game.update();
            game.render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    game.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}