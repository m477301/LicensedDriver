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
        Position += velocity * glm::vec3(0.0f, 0.0f, 1.0f);;
    }
    if (direction == BACKWARD) { 
        Position -= velocity * glm::vec3(0.0f, 0.0f, 1.0f);;;
    }
    if (direction == LEFT) {
        //std::cout << "LEFT" << std::endl;
        Rotation += 1.0f;
        //// calculate the new Front vector
        //glm::vec3 front;
        //front.x = cos(glm::radians(Yaw));
        //front.z = sin(glm::radians(Yaw));
        //cameraFront = glm::normalize(front);
        //// also re-calculate the Right and Up vector
        //Right = glm::normalize(glm::cross(cameraFront, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //cameraUp = glm::normalize(glm::cross(Right, cameraFront));
    }
    if (direction == RIGHT) {
        //std::cout << "RIGHT" << std::endl;
        Rotation -= 1.0f;
    }
}