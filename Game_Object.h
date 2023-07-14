/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Sprite_Renderer.h"


// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    glm::vec3   Position, Size, Velocity;
    glm::vec3   Color;
    float       Rotation;
    bool        IsSolid;
    bool        Destroyed;
    // render state
    Texture2D   Sprite;
    // constructor(s)
    GameObject();
    GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
    // draw sprite
    virtual void Draw(SpriteRenderer& renderer);
};

#endif