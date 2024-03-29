#include "Game.h"
#include "Resource_Manager.h"
#include "Camera.h"
#include "Sprite.h"
#include "Model.h"
#include "StopSign.h"
#include "Obstacle.h"
#include "Player.h"
#include "Skybox.h"

#include <array>
#include <vector>

// Game-related State data
TextRenderer* Text;

// Background
Skybox* skybox;

// Camera
Camera* camera;

// SCENE
Scene* scene;

// Player
Player* Car;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height), Points(20)
{
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
    Camera camera;
}

Game::~Game()
{
    delete scene;
    delete camera;
    delete Car;
    delete skybox;
}

void Game::Init()
{
    // Text
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/ocraext.TTF", 24);

    // load shaders
    Shader defaultShader, modelShader, lightShader, skyboxShader;
    defaultShader = ResourceManager::LoadShader("shaders/default_v.txt", "shaders/default_f.txt", nullptr, "shader");
    lightShader = ResourceManager::LoadShader("shaders/lightCube_v.txt", "shaders/lightCube_f.txt", nullptr, "light_cube");
    skyboxShader = ResourceManager::LoadShader("shaders/skybox_v.txt", "shaders/skybox_f.txt", nullptr, "skybox");

    scene = new Scene(defaultShader, lightShader);

    glm::vec3 carPos = glm::vec3(0.0f, 0.0f, 0.0f);
    Car = new Player("objects/car/legend.obj", defaultShader, carPos);

    // Setup Camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    camera = new Camera(cameraPos);

    // Background
    skybox = new Skybox();
}

void Game::Update(float dt)
{

    Car->update(dt);

    scene->Update(dt);

    this->checkInfractions();

    if (this->State == GAME_ACTIVE && this->IsCompleted())
    {
        this->ResetGame();
        this->State = GAME_WIN;
    }
}

void Game::Render(float dt)
{
    if (this->State == GAME_WIN) {
        Text->RenderText("YOU PASSED!", (this->Width / 2.0f) - (300.0f), this->Height / 2, 3.0f);
        Text->RenderText("Press M to MENU", (this->Width / 2.0f) - (275.0f), this->Height / 2 + 80.0f, 2.0f);

    }
    if (this->State == GAME_MENU) {
        Text->RenderText("LICENSED DRIVER", (this->Width/2.0f)-(300.0f), this->Height / 2, 3.0f);
        Text->RenderText("Press ENTER to start", (this->Width/2.0f)-(275.0f), this->Height / 2 + 80.0f, 2.0f);
    }
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU)
    {
        glDepthRange(0, 0.01);

        // TEXT
        std::stringstream ss; ss << this->Points;
        Text->RenderText("Points:" + ss.str(), 5.0f, 5.0f, 1.0f);

        glDepthRange(0.01, 1.0);

        ResourceManager::GetShader("shader").Use().SetVector3f("viewPos", camera->Position);

        glm::mat4 view = camera->GetViewMatrix(Car->Position, Car->Rotation);
        // Camera Info
        glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        ResourceManager::GetShader("shader").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("shader").SetMatrix4("view", view);
        ResourceManager::GetShader("shader").SetMatrix4("model", model);


        // DRAW SCENE
        scene->Render(Car->Position);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));

        // Draw Background
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        ResourceManager::GetShader("skybox").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("skybox").SetMatrix4("view", view);
        ResourceManager::GetShader("skybox").SetMatrix4("model", model);

        skybox->Draw();

        // DRAW PLAYER
        Car->Draw(dt);

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

        if (!this->Keys[GLFW_KEY_UP] && !this->Keys[GLFW_KEY_DOWN])
            Car->forwardDirection = 0;
        if (!this->Keys[GLFW_KEY_RIGHT] && !this->Keys[GLFW_KEY_LEFT]) {
            Car->rotationDirection = 0;
        }
    }
    if (this->State == GAME_MENU) {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
        }
    }
    if (this->State == GAME_WIN) {
        if (this->Keys[GLFW_KEY_M] && !this->KeysProcessed[GLFW_KEY_M]) {
            this->State = GAME_MENU;
            this->KeysProcessed[GLFW_KEY_M] = true;
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

void Game::ResetGame() {
    this->Points = 20;
    Car->Position = glm::vec3(0.0f, 0.0f, 0.0f);
    // Reset Camera
    camera->vantagePoint = INSIDE_CAR;
    camera->Front = glm::vec3(0.0f, 1.0f, 1.0f);
    camera->Zoom = 80.0f;

    // Reset All Obstacles
    scene->StopSignObstacle->correctManoeuvre = false;
    scene->StopSignObstacle->PassedObjective = false;
}

bool Game::IsCompleted() {
    if (scene->StopSignObstacle->correctManoeuvre && scene->StopSignObstacle->PassedObjective) {
        return true;
    }
    return false;
}

void Game::checkInfractions() {
    if (this->Points <= 0) {
        this->Points = 0;
    }
    else {
        this->Points -= scene->checkInfractions(Car->Position, Car->Velocity);
        if (this->Points <= 0) {
            this->ResetGame();
            this->State = GAME_MENU;
        }
    }
}

