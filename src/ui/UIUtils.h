#pragma once
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

inline bool isMouseOver(float x, float y, float w, float h, double mx, double my) {
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}
inline void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}