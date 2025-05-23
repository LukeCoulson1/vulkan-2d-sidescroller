#pragma once

class Enemy {
public:
    Enemy();
    ~Enemy();

    void move(float deltaTime);
    void attack();
    void render();
    void update();
    void cleanup();

private:
    float positionX;
    float positionY;
    int health;
};