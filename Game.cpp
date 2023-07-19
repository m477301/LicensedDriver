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
#include "Sprite.h"
#include "Game_Object.h"
#include "Model.h"

// Game-related State data
Sprite* Road;
Camera* camera;
GameObject* Car;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
    Camera camera;
}

Game::~Game()
{
    delete Road;
    delete camera;
}

void Game::Init()
{

    // load shaders
    ResourceManager::LoadShader("default_v.txt", "default_f.txt", nullptr, "defaultShader");
    ResourceManager::LoadShader("model_v.txt", "model_f.txt", nullptr, "modelShader");
    // set render-specific controls
    Shader myShader, modelShader;
    myShader = ResourceManager::GetShader("defaultShader");
    Road = new Sprite(myShader);
    modelShader = ResourceManager::GetShader("modelShader");
    glm::vec3 carPos = glm::vec3(0.0f, 0.0f, 0.0f);
    Car = new GameObject("objects/car/CarC6_0003.obj", modelShader, carPos);
    // load textures
    ResourceManager::LoadTexture("textures/road.jpg", false, "road");

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    // Setup Camera
    camera = new Camera(cameraPos);
}

void Game::Update(float dt)
{

}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        glm::mat4 view = camera->GetViewMatrix(Car->Position, Car->Rotation);
        // Camera Info
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
        ResourceManager::GetShader("defaultShader").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("defaultShader").SetMatrix4("view", view);

        // draw background
        //Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        Texture2D myTexture;
        myTexture = ResourceManager::GetTexture("road");
        Road->DrawSprite(myTexture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(this->Width, this->Height, 0.0f), 0.0f);

        //// Draw Model
        ResourceManager::GetShader("modelShader").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("modelShader").SetMatrix4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(Car->Position) + glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(Car->Rotation), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
        ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
        Car->Draw();
    }
}

void Game::KeyboardInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        // CAMERA
        if (this->Keys[GLFW_KEY_W])
            camera->ProcessKeyboard(FORWARD, dt);
        if (this->Keys[GLFW_KEY_S])
            camera->ProcessKeyboard(BACKWARD, dt);
        if (this->Keys[GLFW_KEY_A])
            camera->ProcessKeyboard(LEFT, dt);
        if (this->Keys[GLFW_KEY_D])
            camera->ProcessKeyboard(RIGHT, dt);
        if (this->Keys[GLFW_KEY_C])
            camera->changeViewPosition();

        // CAR
        if (this->Keys[GLFW_KEY_UP])
            Car->move(FORWARD, dt);
        if (this->Keys[GLFW_KEY_DOWN])
            Car->move(BACKWARD, dt);
        if (this->Keys[GLFW_KEY_RIGHT]) {
            std::cout << "CAR " << Car->Position.x << " " << Car->Position.y << " " << Car->Position.z << "\n" << std::endl;
            std::cout << "CAMERA " << camera->Position.x << " " << camera->Position.y << " " << camera->Position.z << "\n" << std::endl;
            Car->move(RIGHT, dt);
        }
        if (this->Keys[GLFW_KEY_LEFT]) {
            std::cout << "CAR " << Car->Position.x << " " << Car->Position.y << " " << Car->Position.z << "\n" << std::endl;
            std::cout << "CAMERA " << camera->Position.x << " " << camera->Position.y << " " << camera->Position.z << "\n" << std::endl;
            Car->move(LEFT, dt);
        }
    }
}

void Game::MouseInput(float xpos, float ypos)
{
    //if (this->State == GAME_ACTIVE)
    //{
    //    if (firstMouse)
    //    {
    //        this->lastX = xpos;
    //        this->lastY = ypos;
    //        this->firstMouse = false;
    //    }

    //    float xoffset = xpos - this->lastX;
    //    float yoffset = this->lastY - ypos; // reversed since y-coordinates go from bottom to top

    //    this->lastX = xpos;
    //    this->lastY = ypos;

    //    camera->ProcessMouseMovement(xoffset, yoffset);
    //}
}

void Game::ScrollInput(float yoffset) {
    //camera->ProcessMouseScroll(yoffset);
}
