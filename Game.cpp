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

#include <array>
#include <vector>

// Game-related State data
Camera* camera;

TextRenderer* Text;

Sprite* Road;
//Sprite* CarVertTest;
//Sprite* MStartVertTest;
//Sprite* MEndVertTest;

GameObject* Car;
GameObject* StopSign;
GameObject* StopMarkings;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Points(20)
{
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
    Camera camera;
}

Game::~Game()
{
    delete Road;
    delete camera;
    delete Car;
    delete StopSign;
    delete StopMarkings;
    //delete CarVertTest;
    //delete MStartVertTest;
    //delete MEndVertTest;
}

void Game::Init()
{
    // Text
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/ocraext.TTF", 24);
    // load shaders
    ResourceManager::LoadShader("default_v.txt", "default_f.txt", nullptr, "defaultShader");
    ResourceManager::LoadShader("model_v.txt", "model_f.txt", nullptr, "modelShader");
    // set render-specific controls
    Shader myShader, modelShader;

    std::vector<float> roadVertices = {
        // positions            // normals         // texcoords
         3.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,  1.5f,  0.0f,  // bottom left
        -3.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f, // top left
        -3.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f, // top right 
         3.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,  1.5f,  0.0f,  // bottom left
        -3.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f, //  top right
         3.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,  1.5f, 10.0f  // bottom right
    };
    myShader = ResourceManager::GetShader("defaultShader");
    Road = new Sprite(myShader, roadVertices);

    //std::vector<float> verticalVertices = {
    //    // positions            // normals         // texcoords
    //     1.0f, 2.0f,  0.0f,  0.0f, 1.0f, 0.0f,  1.5f,  0.0f,  // top right
    //     1.0f, 0.0f,  0.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f, // bottom right
    //    -1.0f, 2.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f, // top left

    //     1.0f, 0.0f,  0.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f, // bottom right
    //    -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f, // bottom left
    //    -1.0f, 2.0f, 0.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f, // top left
    //};
    //CarVertTest = new Sprite(myShader, verticalVertices);
    //MStartVertTest = new Sprite(myShader, verticalVertices);
    //MEndVertTest = new Sprite(myShader, verticalVertices);

    modelShader = ResourceManager::GetShader("modelShader");
    glm::vec3 carPos = glm::vec3(0.0f, 0.0f, 0.0f);
    Car = new GameObject("objects/car/CarC6_0003.obj", modelShader, carPos);

    glm::vec3 stopSignPos = glm::vec3(-2.0f, 0.0f, 10.0f);
    StopSign = new GameObject("objects/stopsign/stopsign.obj", modelShader, stopSignPos);

    glm::vec3 stopMarkingsPos = glm::vec3(0.0f, 0.1f, 10.0f - 1.0f);
    StopMarkings = new GameObject("objects/stopsign/stopmarkings.obj", modelShader, stopMarkingsPos);

    // load textures
    ResourceManager::LoadTexture("textures/road.jpg", false, "road");
    ResourceManager::LoadTexture("textures/white.jpg", false, "awesome");

    // Setup Camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    camera = new Camera(cameraPos);
}

void Game::Update(float dt)
{
    Car->Update(dt);
    // Check if any road infractions occured
    this->checkInfractions();


}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        glDepthRange(0, 0.01);

        // TEXT
        std::stringstream ss; ss << this->Points;
        Text->RenderText("Points:" + ss.str(), 5.0f, 5.0f, 1.0f);

        glDepthRange(0.01, 1.0);

        glm::mat4 view = camera->GetViewMatrix(Car->Position, Car->Rotation);
        // Camera Info
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        ResourceManager::GetShader("defaultShader").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("defaultShader").SetMatrix4("view", view);
        ResourceManager::GetShader("defaultShader").SetMatrix4("model", model);

        // draw road
        Texture2D myTexture;
        myTexture = ResourceManager::GetTexture("road");
        Road->DrawSprite(myTexture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(this->Width, this->Height, 0.0f), 0.0f);

        ////// draw test vertices
        //myTexture = ResourceManager::GetTexture("awesome");

        //glm::vec3 carFrontPos = Car->Position + glm::vec3(0.0f, 0.0f, (157.30f * 0.01f)/2.0f);
        //CarVertTest->DrawSprite(myTexture, Car->Position, glm::vec3(this->Width, this->Height, 0.0f), 0.0f);
        //model = glm::translate(model, carFrontPos);
        //ResourceManager::GetShader("defaultShader").SetMatrix4("model", model);
        //model = glm::mat4(1.0f);

        //glm::vec3 linePos = StopMarkings->Position + glm::vec3(0.0f, 0.0f, 2.17f*0.32f);
        //MStartVertTest->DrawSprite(myTexture, linePos, glm::vec3(this->Width, this->Height, 0.0f), 0.0f);
        //model = glm::translate(model, linePos);
        //ResourceManager::GetShader("defaultShader").SetMatrix4("model", model);
        //model = glm::mat4(1.0f);

        //glm::vec3 lineLPos = StopMarkings->Position + glm::vec3(0.0f, 0.0f, (2.17f * 0.32f)-1.5f);
        ////std::cout << " MARKPOS " << lineLPos.x << " , " << lineLPos.y << " , " << lineLPos.z << std::endl;
        //MEndVertTest->DrawSprite(myTexture, lineLPos, glm::vec3(this->Width, this->Height, 0.0f), 0.0f);
        //model = glm::translate(model, lineLPos);
        //ResourceManager::GetShader("defaultShader").SetMatrix4("model", model);
        //model = glm::mat4(1.0f);

        // Draw Model
        ResourceManager::GetShader("modelShader").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("modelShader").SetMatrix4("view", view);

        // draw Stop Sign
        model = glm::translate(model, glm::vec3(StopSign->Position)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));	// it's a bit too big for our scene, so scale it down
        ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
        StopSign->Draw();

        model = glm::mat4(1.0f);

        // draw Stop Markings
        model = glm::translate(model, glm::vec3(StopMarkings->Position)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
        model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));	// it's a bit too big for our scene, so scale it down
        ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
        StopMarkings->Draw();

        model = glm::mat4(1.0f);

        // draw Car
        model = glm::translate(model, glm::vec3(Car->Position)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(Car->Rotation), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
        ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
        Car->Draw();

        // restore depth range
        glDepthRange(0, 1.0);
    }
}

void Game::KeyboardInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        // CAMERA
        if (this->Keys[GLFW_KEY_C] && !this->KeysProcessed[GLFW_KEY_C]) 
        {
            camera->changeViewPosition
            (
                camera->vantagePoint == INSIDE_CAR 
                ? camera->vantagePoint = OUTSIDE_CAR 
                : 
                (
                    camera->vantagePoint == OUTSIDE_CAR ? 
                    camera->vantagePoint = FREE_ROAMING 
                    : 
                    camera->vantagePoint = INSIDE_CAR
                )
            );
            this->KeysProcessed[GLFW_KEY_C] = true;
        }
        if (camera->vantagePoint == FREE_ROAMING) {
            if (this->Keys[GLFW_KEY_W])
                camera->ProcessKeyboard(FORWARD, dt);
            if (this->Keys[GLFW_KEY_S])
                camera->ProcessKeyboard(BACKWARD, dt);
            if (this->Keys[GLFW_KEY_A])
                camera->ProcessKeyboard(LEFT, dt);
            if (this->Keys[GLFW_KEY_D])
                camera->ProcessKeyboard(RIGHT, dt);
        }

        // CAR
        if (this->Keys[GLFW_KEY_UP])
            Car->move(FORWARD, dt);
        if (this->Keys[GLFW_KEY_DOWN])
            Car->move(BACKWARD, dt);
        if (this->Keys[GLFW_KEY_RIGHT]) {
            Car->move(RIGHT, dt);
        }
        if (this->Keys[GLFW_KEY_LEFT]) {
            Car->move(LEFT, dt);
        }
    }
}

void Game::MouseInput(float xpos, float ypos)
{
    if (this->State == GAME_ACTIVE && (camera->vantagePoint == INSIDE_CAR || camera->vantagePoint == FREE_ROAMING ))
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

        camera->ProcessMouseMovement(xoffset, yoffset, true);
    }
}

void Game::checkInfractions() {

    // Check for stop sign infraction

    if (Car->Position.z + ((157.30f * 0.01f) / 2.0f) < StopMarkings->Position.z + (2.17f * 0.32f)
        &&
        Car->Position.z + ((157.30f * 0.01f) / 2.0f) > StopMarkings->Position.z + (2.17f * 0.32f) - 1.5f) {
        // check if car reduces speed to 0
        if (Car->Velocity.x + Car->Velocity.y + Car->Velocity.z == 0.0f) {
            // they stopped
            StopMarkings->PassedObjective = true;
        }
    }
    else if (
        // obstacle has been passed and infraction has been committed
        (Car->Position.z + ((157.30f * 0.01f) / 2.0f) > StopMarkings->Position.z + (2.17f * 0.32f))
        &&
        !StopMarkings->PassedObjective
        ) {
        this->Points -= 5;
        StopMarkings->PassedObjective = true;
    }
    else if(Car->Position.z + ((157.30f * 0.01f) / 2.0f) < StopMarkings->Position.z + (2.17f * 0.32f)) {
        StopMarkings->PassedObjective = false;
    }
}