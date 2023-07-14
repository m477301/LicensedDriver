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
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// Game-related State data
SpriteRenderer* RoadRenderer;


Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("default_v.txt", "default_f.txt", nullptr, "defaultShader");
    //ResourceManager::LoadShader("model_v.txt", "model_f.txt", nullptr, "modelShader");
    // configure shaders
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ResourceManager::GetShader("defaultShader").Use().setMat4("projection", projection);
    ResourceManager::GetShader("defaultShader").setMat4("view", view);
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

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
        if (this->Keys[GLFW_KEY_ESCAPE])
            glfwSetWindowShouldClose(window, true);

        if (this->Keys[GLFW_KEY_W])
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (this->Keys[GLFW_KEY_S])
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (this->Keys[GLFW_KEY_A])
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (this->Keys[GLFW_KEY_D])
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // draw background
        //Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        Texture2D myTexture;
        myTexture = ResourceManager::GetTexture("road");
        Renderer->DrawSprite(myTexture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        // draw player
        Player->Draw(*Renderer);
    }
}