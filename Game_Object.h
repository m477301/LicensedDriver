
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Sprite.h"
#include "Model.h"
#include "Game.h"

const float CAR_SPEED = 2.5f;
const float CAR_ACCELERATION = 1.5;

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    bool PassedObjective = false;
    bool infractionCommitted = false;
    // object state
    glm::vec3   Position, Size, Velocity, Acceleration, Speed;
    glm::vec3   initialVelocity, PreviousPosition;
    glm::vec3   changeInPosition, changeInVelocity;
    glm::vec3   Color;
    float       Rotation;
    float       RotationSpeed = 0.5;
    float       MovementSpeed;
    bool        IsSolid;
    bool        Destroyed;
    // render state
    Texture2D   Object;
    Model* model;

    glm::vec3 carFront = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

    // constructor(s)
    GameObject();
    GameObject(string const& path, Shader &Shader, glm::vec3 pos, glm::vec3 velocity = glm::vec3(0.0f,0.0f,0.0f));
    GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f));
    // Update Sprite
    void Update(float deltaTime);
    // draw sprite
    virtual void Draw(Sprite& sprite);
    virtual void Draw();
    // move sprite
    void move(Movement direction, float deltaTime);
};

#endif