#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "Game_Level.h"
#include <iostream>

#include "Texture.h"
#include "Shader.h"
#include "Resource_Manager.h"
#include "Sprite.h"
#include "Obstacle.h"
#include "StopSign.h"


class Scene
{
public:
    // SHADER
    Shader shader, modelShader, lightShader;

    // OBJECTS
    Sprite* Road;

    // Obstacles
    StopSign* StopSignObstacle;

    unsigned int lightCubeVAO, cubeVAO;

    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    // constructor/destructor
    Scene(Shader& shader, Shader& modelShader, Shader& lightShader);
    ~Scene();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void Update(float dt);
    void Render();
    int checkInfractions(glm::vec3 carPosition, glm::vec3 carVelocity);
};

#endif