
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Sprite.h"
#include "Model.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    glm::vec3   Position, Size;
    Model* model;

    // constructor(s)
    GameObject();
    GameObject(string const& path, Shader &Shader, glm::vec3 pos);
    // Update Sprite
    void update(float deltaTime);
    // draw sprite
    //virtual void Draw(Sprite& sprite);
    virtual void Draw();
};

#endif