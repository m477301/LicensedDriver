#include "Scene.h";

#include <iostream>

// constructor/destructor
Scene::Scene(Shader &shader, Shader &modelShader, Shader& lightShader)
{
	this->shader = shader;
	this->modelShader = modelShader;
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

	Road = new Sprite
					(
						this->shader, 
						glm::vec3(3.0f, 0.0f, 10.0f),
						glm::vec3(-3.0f, 0.0f, 10.0f),
						glm::vec3(-3.0f, 0.0f, -10.0f),
						glm::vec3(3.0f, 0.0f, -10.0f)
					);

	glm::vec3 stopSignPos = glm::vec3(0.0f, 0.0f, 5.0f);
	this->StopSignObstacle = new StopSign(STOP_SIGN, "objects/stopsign/stopsign.obj", this->shader, stopSignPos);

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
	//this->checkInfractions();
}

void Scene::Render(glm::mat4 view, glm::mat4 projection, glm::mat4 model)
{
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

	ResourceManager::GetShader("shader").SetMatrix4("projection", projection);
	ResourceManager::GetShader("shader").SetMatrix4("view", view);
	ResourceManager::GetShader("shader").SetMatrix4("model", model);

	// draw road
	model = glm::mat4(1.0f);
	Texture2D diffuseRoadTexture, specularRoadTexture, normalRoadTexture;
	diffuseRoadTexture = ResourceManager::GetTexture("road");
	specularRoadTexture = ResourceManager::GetTexture("road_specular");
	normalRoadTexture = ResourceManager::GetTexture("road_normal");
	this->Road->DrawSprite(diffuseRoadTexture, specularRoadTexture, normalRoadTexture, glm::vec3(0.0f, 0.0f, 0.0f));
	ResourceManager::GetShader("shader").SetMatrix4("model", model);
	model = glm::mat4(1.0f);

	// Draw Obstcales
	model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));//
	//model = glm::translate(model, glm::vec3(StopSignObstacle->Position));
	ResourceManager::GetShader("shader").SetMatrix4("model", model);
	this->StopSignObstacle->Render();


	// Draw Buildings

	//model = glm::mat4(1.0f);
	//// draw Stop Sign
	//model = glm::translate(model, glm::vec3(StopSign->Position)); // translate it down so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));	// it's a bit too big for our scene, so scale it down
	//StopSign->Draw();

	//model = glm::mat4(1.0f);

	//// draw Stop Markings
	//model = glm::translate(model, glm::vec3(StopMarkings->Position)); // translate it down so it's at the center of the scene
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
		//model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));	// it's a bit too big for our scene, so scale it down
	//ResourceManager::GetShader("modelShader").SetMatrix4("model", model);
	//StopMarkings->Draw();
}