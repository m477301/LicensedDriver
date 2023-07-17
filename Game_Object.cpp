#include "Game_Object.h"
#include <iostream>

GameObject::GameObject()
    : Position(0.0f, 0.0f, 0.0f), Size(10.0f, 10.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Object(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Object(sprite), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(string const& path, Shader& Shader, glm::vec3 pos, glm::vec3 velocity) : Position(pos), Velocity(velocity), model(new Model(path, Shader)),Rotation(0.0f), MovementSpeed(CAR_SPEED) 
    { }

void GameObject::Draw(Sprite& sprite)
{
    sprite.DrawSprite(this->Object, this->Position, this->Size, this->Rotation, this->Color);
}

void GameObject::Draw()
{
    this->model->Draw();
}

void GameObject::move(Movement direction, float deltaTime)
{

    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        Position += carFront * velocity;
    }
    if (direction == BACKWARD) { 
        Position -= carFront * velocity;
    }
    if (direction == LEFT) {
        Rotation += 1.0f;
        glm::vec3 front;
        front.x = sin(glm::radians(Rotation));
        front.y = 0.0f;
        front.z = cos(glm::radians(Rotation));
        carFront = glm::normalize(front);
    }
    if (direction == RIGHT) {
        Rotation -= 1.0f;
        glm::vec3 front;
        front.x = sin(glm::radians(Rotation));
        front.y = 0.0f;
        front.z = cos(glm::radians(Rotation));
        carFront = glm::normalize(front);
    }
}