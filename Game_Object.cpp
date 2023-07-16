#include "Game_Object.h"

GameObject::GameObject()
    : Position(0.0f, 0.0f, 0.0f), Size(10.0f, 10.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Object(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Object(sprite), IsSolid(false), Destroyed(false) { }

void GameObject::Draw(Sprite& sprite)
{
    sprite.DrawSprite(this->Object, this->Position, this->Size, this->Rotation, this->Color);
}