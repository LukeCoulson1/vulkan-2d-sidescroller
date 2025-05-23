#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <string>

class Button {
public:
    float x, y, w, h;
    GLuint texture;
    std::string label;
    bool hovered = false;

    Button(float x, float y, float w, float h, GLuint texture, const std::string& label = "")
        : x(x), y(y), w(w), h(h), texture(texture), label(label) {}

    void draw() const {
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
        // Optionally draw label here
    }

    bool isMouseOver(double mx, double my) const {
        return mx >= x && mx <= x + w && my >= y && my <= y + h;
    }
};