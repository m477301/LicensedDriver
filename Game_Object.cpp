#include "Game_Object.h"
#include <iostream>

GameObject::GameObject()
    : Position(0.0f, 0.0f, 0.0f), Size(10.0f, 10.0f, 1.0f) { }

GameObject::GameObject(string const& path, Shader& Shader, glm::vec3 pos) : Position(pos), model(new Model(path, Shader))
    { }

void GameObject::Draw()
{
    this->model->Draw();
}


void GameObject::Update(float deltaTime) {
}
