#include "Game.h"
#include "Resource_Manager.h"
#include "Camera.h"
#include "Sprite.h"
#include "Game_Object.h"
#include "Model.h"
#include "StopSign.h"
#include "Obstacle.h"
#include "Player.h"

#include <array>
#include <vector>

// Game-related State data
TextRenderer* Text;

// Camera
Camera* camera;

// SCENE
Scene* scene;

// Player
Player* Car;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Points(20)
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
}

void Game::Init()
{
    // Text
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/ocraext.TTF", 24);

    // load shaders
    Shader defaultShader, modelShader, lightShader;
    defaultShader = ResourceManager::LoadShader("shaders/normal_v.txt", "shaders/normal_f.txt", nullptr, "shader");
    //modelShader = ResourceManager::LoadShader("model_v.txt", "model_f.txt", nullptr, "modelShader");
    lightShader = ResourceManager::LoadShader("shaders/lightCube_v.txt", "shaders/lightCube_f.txt", nullptr, "light_cube");

    scene = new Scene(defaultShader, defaultShader, lightShader);

    glm::vec3 carPos = glm::vec3(0.0f, 0.0f, 0.0f);
    Car = new Player("objects/car/CarC6_0003.obj", defaultShader, carPos);

    // Setup Camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    camera = new Camera(cameraPos);

}

void Game::Update(float dt)
{
    Car->update(dt);

    scene->Update(dt);

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

        ResourceManager::GetShader("shader").Use().SetVector3f("viewPos", camera->Position);

        // DRAW SCENE

        scene->Render(view, projection, model);
        
        model = glm::mat4(1.0f);

        // DRAW PLAYER
        ResourceManager::GetShader("shader").Use().SetMatrix4("projection", projection);
        ResourceManager::GetShader("shader").SetMatrix4("view", view);

        // draw Car
        model = glm::translate(model, glm::vec3(Car->Position)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(Car->Rotation), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
        ResourceManager::GetShader("shader").SetMatrix4("model", model);
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

    
    if (this->Points <= 0) {
        this->Points = 0;
    }
    else {
        this->Points -= scene->checkInfractions(Car->Position, Car->Velocity);
    }
}

