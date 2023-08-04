#include "Scene.h";

#include <iostream>

// constructor/destructor
Scene::Scene(Shader &shader, Shader &lightShader)
{
	this->shader = shader;
	this->lightShader = lightShader;
	this->Init();
}

Scene::~Scene() 
{
	delete this->Road;
	delete this->StopSignObstacle;
	glDeleteVertexArrays(1, &this->lightCubeVAO);
}
// initialize game state (load all shaders/textures/levels)
void Scene::Init()
{

	// DIRECTIONAL LIGHT
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &this->lightCubeVAO);
	glBindVertexArray(this->lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Road = new Sprite(
		this->shader,
		glm::vec3(3.0f, 0.0f, 10.0f),
		glm::vec3(-3.0f, 0.0f, 10.0f),
		glm::vec3(-3.0f, 0.0f, -10.0f),
		glm::vec3(3.0f, 0.0f, -10.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	);

	glm::vec3 stopSignPos = glm::vec3(-2.5f, 0.0f, 5.0f);
	this->StopSignObstacle = new StopSign(STOP_SIGN, "objects/stopsign/stopsign.obj", this->shader, this->shader, stopSignPos);

	//// load textures

	ResourceManager::LoadTexture("textures/road/BaseColor.png", false, "road");
	ResourceManager::LoadTexture("textures/road/Specular.png", false, "road_specular");
	ResourceManager::LoadTexture("textures/road/Normal.png", false, "road_normal");

	ResourceManager::LoadTexture("textures/stop/AO_Transparent.png", true, "stop");
	ResourceManager::LoadTexture("textures/stop/Specular_Transparent.png", true, "stop_specular");
	ResourceManager::LoadTexture("textures/stop/Normal_Transparent.png", true, "stop_normal");

	// Set Materials
	ResourceManager::GetShader("shader").Use().SetInteger("material.diffuse", 0);
	ResourceManager::GetShader("shader").SetInteger("material.specular", 1);
	ResourceManager::GetShader("shader").SetInteger("material.normal", 2);
}
// game loop
void Scene::Update(float dt)
{
	// Check if any road infractions occured
	// for all obstacles in scene check infractions
	//this->checkInfractions();
}

void Scene::Render(glm::vec3 carPosition)
{
	glm::mat4 model = glm::mat4(1.0f);

	// be sure to activate shader when setting uniforms/drawing objects
//ResourceManager::GetShader("defaultShader").Use().SetVector3f("light.direction", -0.2f, -1.0f, -0.3f);
	ResourceManager::GetShader("shader").Use().SetVector3f("lightPos", this->lightPos);

	// light Properties
	ResourceManager::GetShader("shader").SetVector3f("light.position", this->lightPos);
	ResourceManager::GetShader("shader").SetVector3f("light.position", this->lightPos);
	ResourceManager::GetShader("shader").SetVector3f("light.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
	ResourceManager::GetShader("shader").SetVector3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	ResourceManager::GetShader("shader").SetVector3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// Material Properties
	ResourceManager::GetShader("shader").SetFloat("material.shininess", 32.0f);
	ResourceManager::GetShader("shader").SetMatrix4("model", model);

	// draw road
	model = glm::mat4(1.0f);
	Texture2D diffuseRoadTexture, specularRoadTexture, normalRoadTexture;
	diffuseRoadTexture = ResourceManager::GetTexture("road");
	specularRoadTexture = ResourceManager::GetTexture("road_specular");
	normalRoadTexture = ResourceManager::GetTexture("road_normal");
	//model = glm::translate(model, glm::vec3(Road->Position.x, 0.0f,carPosition.z));
	ResourceManager::GetShader("shader").SetMatrix4("model", model); 
	this->Road->DrawSprite(diffuseRoadTexture, specularRoadTexture, normalRoadTexture, glm::vec3(0.0f, 0.0f, 0.0f));

	// Draw Obstcales
	model = glm::mat4(1.0f);
	ResourceManager::GetShader("shader").SetMatrix4("model", model);
	this->StopSignObstacle->Draw();

}

// Check if any infractions have occured in the scene
int Scene::checkInfractions(glm::vec3 carPosition, glm::vec3 carVelocity) {

	// for all obstacles check infractions
    // Check for stop sign infraction
	int pointsLost = 0;

    if (carPosition.z + ((157.30f * 0.01f) / 2.0f) < StopSignObstacle->Position.z
        &&
		carPosition.z + ((157.30f * 0.01f) / 2.0f) > StopSignObstacle->Position.z - 1.5f) {
        // check if car reduces speed to 0
        if (carVelocity.x + carVelocity.y + carVelocity.z == 0.0f) {
            // they stopped
			StopSignObstacle->correctManoeuvre = true;
        }
    }
    else if (
        // obstacle has been passed and infraction has been committed
        (carPosition.z + ((157.30f * 0.01f) / 2.0f) > StopSignObstacle->Position.z)
        &&
        !StopSignObstacle->PassedObjective
        ) {
		if (!StopSignObstacle->correctManoeuvre) {
			pointsLost += 5;
		}
		StopSignObstacle->PassedObjective = true;
    }
    else if(carPosition.z + ((157.30f * 0.01f) / 2.0f) < StopSignObstacle->Position.z) {
		StopSignObstacle->PassedObjective = false;
    }

	return pointsLost;
}