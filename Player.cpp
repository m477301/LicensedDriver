#include "Player.h"

Player::Player(string const& path, Shader& shader, glm::vec3 pos)
	: GameObject(path, shader, pos) {
    this->MovementSpeed = CAR_SPEED;
}

void Player::update(float deltaTime) {
    this->changeInPosition = this->Position - this->PreviousPosition;
    this->PreviousPosition = this->Position;
    this->initialVelocity = this->Velocity;
    this->Velocity = ((this->changeInPosition) / deltaTime);
    this->changeInVelocity = this->Velocity - this->initialVelocity;
    this->Acceleration = CAR_ACCELERATION * (this->changeInVelocity / deltaTime);
}

void Player::move(Movement direction, float deltaTime) {
    float speed = MovementSpeed * deltaTime;
    // Velocity in all axes is
    if (direction == FORWARD) {
        this->Position += carFront * (speed);
    }
    if (direction == BACKWARD) {
        this->Position -= carFront * (speed);
    }
    if (direction == LEFT) {
        this->Rotation += this->RotationSpeed * 0.5;
        glm::vec3 front;
        front.x = sin(glm::radians(Rotation));
        front.y = 0.0f;
        front.z = cos(glm::radians(Rotation));
        this->carFront = glm::normalize(front);
    }
    if (direction == RIGHT) {
        this->Rotation -= this->RotationSpeed;
        glm::vec3 front;
        front.x = sin(glm::radians(Rotation));
        front.y = 0.0f;
        front.z = cos(glm::radians(Rotation));
        this->carFront = glm::normalize(front);
    }
}