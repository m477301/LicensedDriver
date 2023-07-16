/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "Game.h"
#include "Resource_Manager.h"
#include "Camera.h"
#include "Sprite_renderer.h"
#include "Game_Object.h"

// Game-related State data
SpriteRenderer* RoadRenderer;
Camera* camera;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
    Camera camera;
}

Game::~Game()
{
    delete RoadRenderer;
    delete camera;
    //delete Player;
}

void Game::Init()
{
    // Setup Camera
    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // load shaders
    ResourceManager::LoadShader("default_v.txt", "default_f.txt", nullptr, "defaultShader");
    //ResourceManager::LoadShader("model_v.txt", "model_f.txt", nullptr, "modelShader");
    // configure shaders
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    ResourceManager::GetShader("defaultShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("defaultShader").SetMatrix4("view", view);
    // set render-specific controls
    Shader myShader;
    myShader = ResourceManager::GetShader("defaultShader");
    RoadRenderer = new SpriteRenderer(myShader);
    // load textures
    ResourceManager::LoadTexture("textures/road.jpg", false, "road");
    // configure game objects
    //glm::vec2 RoadPos = glm::vec3(0.0f, 0.0f, 0.0f);
    //Player = new GameObject(RoadPos, PLAYER_SIZE, ResourceManager::GetTexture("road"));
}

void Game::Update(float dt)
{

}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // Camera Info
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();
        ResourceManager::GetShader("defaultShader").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("defaultShader").SetMatrix4("view", view);

        // draw background
        //Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        Texture2D myTexture;
        myTexture = ResourceManager::GetTexture("road");
        RoadRenderer->DrawSprite(myTexture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(this->Width, this->Height, 0.0f), 0.0f);
        // draw level
        //this->Levels[this->Level].Draw(*Renderer);
        // draw player
        //Player->Draw(*Renderer);
    }
}

void Game::KeyboardInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        // KEYBOARD
        //std::cout << "ACTIVE" << std::endl;
        if (this->Keys[GLFW_KEY_W])
            camera->ProcessKeyboard(FORWARD, dt);
        if (this->Keys[GLFW_KEY_S])
            camera->ProcessKeyboard(BACKWARD, dt);
        if (this->Keys[GLFW_KEY_A])
            camera->ProcessKeyboard(LEFT, dt);
        if (this->Keys[GLFW_KEY_D])
            camera->ProcessKeyboard(RIGHT, dt);
    }
}

void Game::MouseInput(float xpos, float ypos)
{
    if (this->State == GAME_ACTIVE)
    {
        if (firstMouse)
        {
            this->lastX = xpos;
            this->lastY = ypos;
            this->firstMouse = false;
        }

        float xoffset = xpos - this->lastX;
        float yoffset = this->lastY - ypos; // reversed since y-coordinates go from bottom to top

        this->lastX = xpos;
        this->lastY = ypos;

        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void Game::ScrollInput(float yoffset) {
    camera->ProcessMouseScroll(yoffset);
}
