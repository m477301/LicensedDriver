#include "Game.h"
#include "Resource_Manager.h"
#include "Camera.h"
#include "Sprite.h"
#include "Game_Object.h"
#include "Model.h"

#include <array>
#include <vector>

//void renderQuad();
//unsigned int loadTexture(const char* path);

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

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

unsigned int lightCubeVAO, cubeVAO;

unsigned int Road_DiffuseMap, Stop_DiffuseMap;
unsigned int Road_SpecularMap, Stop_SpecularMap;
unsigned int Road_NormalMap, Stop_NormalMap;


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
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
}

void Game::Init()
{
    // Text
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/ocraext.TTF", 24);
    // load shaders
    ResourceManager::LoadShader("shaders/normal_v.txt", "shaders/normal_f.txt", nullptr, "defaultShader");
    ResourceManager::LoadShader("model_v.txt", "model_f.txt", nullptr, "modelShader");
    // set render-specific controls
    Shader myShader, modelShader;

    myShader = ResourceManager::GetShader("defaultShader");
    Road = new Sprite(myShader);

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

    //glm::vec3 stopMarkingsPos = glm::vec3(0.0f, 0.01f, 10.0f - 1.0f);
    //StopMarkings = new GameObject("objects/stopsign/stopmarkings.obj", modelShader, stopMarkingsPos);

    //// load textures
    ResourceManager::LoadTexture("textures/road/BaseColor.png", false, "road");
    ResourceManager::LoadTexture("textures/road/Specular.png", false, "road_specular");
    ResourceManager::LoadTexture("textures/road/Normal.png", false, "road_normal");

    //ResourceManager::LoadTexture("textures/white.jpg", false, "awesome");

    // load ROAD textures
    // -------------
    //Road_DiffuseMap = loadTexture("textures/road/BaseColor.png");
    //Road_SpecularMap = loadTexture("textures/road/Specular.png");
    //Road_NormalMap = loadTexture("textures/road/Normal.png");

    // load STOP textures
    //Stop_DiffuseMap = loadTexture("textures/stop/BaseColor.png");
    //Stop_SpecularMap = loadTexture("textures/stop/Specular.png");
    //Stop_NormalMap = loadTexture("textures/stop/Normal.png");

    // Setup Camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    camera = new Camera(cameraPos);

    // SUN Light
    ResourceManager::LoadShader("shaders/lightCube_v.txt", "shaders/lightCube_f.txt", nullptr, "light_cube");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ResourceManager::GetShader("defaultShader").Use().SetInteger("material.diffuse", 0);
    ResourceManager::GetShader("defaultShader").SetInteger("material.specular", 1);
    ResourceManager::GetShader("defaultShader").SetInteger("material.normal", 2);

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

        // be sure to activate shader when setting uniforms/drawing objects
        //ResourceManager::GetShader("defaultShader").Use().SetVector3f("light.direction", -0.2f, -1.0f, -0.3f);
        ResourceManager::GetShader("defaultShader").Use().SetVector3f("viewPos", camera->Position);
        ResourceManager::GetShader("defaultShader").SetVector3f("lightPos", lightPos);

        // light Properties
        ResourceManager::GetShader("defaultShader").SetVector3f("light.position", lightPos);
        ResourceManager::GetShader("defaultShader").SetVector3f("light.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
        ResourceManager::GetShader("defaultShader").SetVector3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        ResourceManager::GetShader("defaultShader").SetVector3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // Material Properties
        ResourceManager::GetShader("defaultShader").SetFloat("material.shininess", 32.0f);
        ResourceManager::GetShader("defaultShader").SetInteger("material.diffuse", 0);
        ResourceManager::GetShader("defaultShader").SetInteger("material.specular", 1);
        ResourceManager::GetShader("defaultShader").SetInteger("material.normal", 2);

        ResourceManager::GetShader("defaultShader").SetMatrix4("projection", projection);
        ResourceManager::GetShader("defaultShader").SetMatrix4("view", view);
        ResourceManager::GetShader("defaultShader").SetMatrix4("model", model);

        //// draw road
        model = glm::mat4(1.0f);
        Texture2D diffuseRoadTexture, specularRoadTexture, normalRoadTexture;
        diffuseRoadTexture = ResourceManager::GetTexture("road");
        specularRoadTexture = ResourceManager::GetTexture("road_specular");
        normalRoadTexture = ResourceManager::GetTexture("road_normal");
        Road->DrawSprite(diffuseRoadTexture, specularRoadTexture, normalRoadTexture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(this->Width, this->Height, 0.0f), 0.0f);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, Stop_DiffuseMap);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, Stop_SpecularMap);
        //glActiveTexture(GL_TEXTURE2);
        //glBindTexture(GL_TEXTURE_2D, Stop_NormalMap);
        //renderQuad();



        // also draw the lamp object
        //ResourceManager::GetShader("light_cube").Use().SetMatrix4("projection", projection);
        //ResourceManager::GetShader("light_cube").SetMatrix4("view", view);
        //model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        //ResourceManager::GetShader("light_cube").SetMatrix4("model", model);

        //glBindVertexArray(lightCubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

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

        model = glm::mat4(1.0f);
        // draw Stop Sign
        model = glm::translate(model, glm::vec3(StopSign->Position)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
        model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));	// it's a bit too big for our scene, so scale it down
        ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
        StopSign->Draw();

        model = glm::mat4(1.0f);

        //// draw Stop Markings
        //model = glm::translate(model, glm::vec3(StopMarkings->Position)); // translate it down so it's at the center of the scene
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
        //model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));	// it's a bit too big for our scene, so scale it down
        //ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
        //StopMarkings->Draw();

        //model = glm::mat4(1.0f);

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
//
//    // Check for stop sign infraction
//
//    if (Car->Position.z + ((157.30f * 0.01f) / 2.0f) < StopMarkings->Position.z + (2.17f * 0.32f)
//        &&
//        Car->Position.z + ((157.30f * 0.01f) / 2.0f) > StopMarkings->Position.z + (2.17f * 0.32f) - 1.5f) {
//        // check if car reduces speed to 0
//        if (Car->Velocity.x + Car->Velocity.y + Car->Velocity.z == 0.0f) {
//            // they stopped
//            StopMarkings->PassedObjective = true;
//        }
//    }
//    else if (
//        // obstacle has been passed and infraction has been committed
//        (Car->Position.z + ((157.30f * 0.01f) / 2.0f) > StopMarkings->Position.z + (2.17f * 0.32f))
//        &&
//        !StopMarkings->PassedObjective
//        ) {
//        this->Points -= 5;
//        StopMarkings->PassedObjective = true;
//    }
//    else if(Car->Position.z + ((157.30f * 0.01f) / 2.0f) < StopMarkings->Position.z + (2.17f * 0.32f)) {
//        StopMarkings->PassedObjective = false;
//    }
}


//// utility function for loading a 2D texture from file
//// ---------------------------------------------------
//unsigned int loadTexture(char const* path)
//{
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }
//
//    return textureID;
//}