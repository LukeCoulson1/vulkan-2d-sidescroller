#pragma once

class Player {
public:
    Player();
    void init();
    void update();
    void move(float deltaX, float deltaY);
    void jump();
    void render();
    void cleanup();

private:
    float positionX;
    float positionY;
    float velocityY;
    bool isJumping;
};