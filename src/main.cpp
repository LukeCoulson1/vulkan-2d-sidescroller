#include <GLFW/glfw3.h>
#include "game/Game.h"
#include "ui/Menu.h"
#include "ui/SettingsMenu.h"
#include "ui/ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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


void updateOrtho(GLFWwindow* window) {
    // Use a fixed virtual resolution
    int virtualWidth = 800;
    int virtualHeight = 600;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, virtualWidth, 0, virtualHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
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
    GLuint newGameTex = ResourceManager::loadTexture("../../img/New_Game.png");
    GLuint settingsTex = loadTexture("../../img/Settings.png");
    GLuint backTex = loadTexture("../../img/New_Game.png"); // Placeholder

    GameState state = GameState::MENU;
    SettingsState settings;

    Game game;
    game.init();

    const int virtualWidth = 800;
    const int virtualHeight = 600;

    // Menu object
    Menu mainMenu;
    mainMenu.addButton((virtualWidth - 200) / 2.0f, 360, 200, 60, newGameTex, "New Game");
    mainMenu.addButton((virtualWidth - 200) / 2.0f, 240, 200, 60, settingsTex, "Settings");

    SettingsMenu settingsMenu(backTex, virtualWidth, virtualHeight);

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

        // Check for mouse button press (single frame detection)
        static bool mousePressed = false;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            mousePressed = true;
        } else {
            mousePressed = false;
        }

        if (state == GameState::MENU) {
            // Menu interaction
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