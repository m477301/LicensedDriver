#include "Game_Object.h"
#include <iostream>

GameObject::GameObject()
    : Position(0.0f, 0.0f, 0.0f), Size(10.0f, 10.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Object(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Object(sprite), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(string const& path, Shader& Shader, glm::vec3 pos, glm::vec3 velocity) : Position(pos), Velocity(velocity), model(new Model(path, Shader)),Rotation(0.0f), MovementSpeed(CAR_SPEED), PreviousPosition(glm::vec3(0.0f,0.0f,0.0f)), Acceleration(CAR_ACCELERATION)
    { }

void GameObject::Draw(Sprite& sprite)
{
    sprite.DrawSprite(this->Object, this->Position, this->Size, this->Rotation, this->Color);
}

void GameObject::Draw()
{
    this->model->Draw();
}


void GameObject::Update(float deltaTime) {

    this->changeInPosition = this->Position - this->PreviousPosition;
    this->PreviousPosition = this->Position;
    this->initialVelocity = this->Velocity;
    this->Velocity = ((this->changeInPosition) / deltaTime);
    this->changeInVelocity = this->Velocity - this->initialVelocity;
    this->Acceleration = CAR_ACCELERATION * (this->changeInVelocity / deltaTime);
}

void GameObject::move(Movement direction, float deltaTime)
{
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