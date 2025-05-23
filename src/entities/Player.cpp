#include "Player.h"

Player::Player() {
    positionX = 0.0f;
    positionY = 0.0f;
    velocityY = 0.0f;
    isJumping = false;
}

void Player::init() {}
void Player::update() {}
void Player::move(float deltaX, float deltaY) {
    positionX += deltaX;
    positionY += deltaY;
}
void Player::jump() {}
void Player::render() {}
void Player::cleanup() {}