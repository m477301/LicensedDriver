#include "StopSign.h"

#include <iostream>

StopSign::StopSign(ObstacleType obstacle, string const& path, Shader& shader, Shader& modelShader, glm::vec3 pos)
	:
	Obstacle(obstacle, path, modelShader, pos)
{ 
	RoadMarking = new Sprite
	(
		shader,
		(pos + glm::vec3(2.0f, 0.01f, -1.4f)),
		(pos + glm::vec3(2.0f, 0.01f, -0.4f)),
		(pos + glm::vec3(0.5f, 0.01f, -0.4f)),
		(pos + glm::vec3(0.5f, 0.01f, -1.4f)),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	);

	Line = new Sprite
	(
		shader,
		(pos + glm::vec3(2.25f, 0.01f, -0.2f)),
		(pos + glm::vec3(2.25f, 0.01f, 0.0f)),
		(pos + glm::vec3(0.25f, 0.01f, 0.0f)),
		(pos + glm::vec3(0.25f, 0.01f, -0.2f)),
		glm::vec2(0.2815f, 0.1909f),
		glm::vec2(0.2827f, 0.1013f),
		glm::vec2(0.2883f, 0.1011f),
		glm::vec2(0.2891f, 0.1838f)
	);
}

void StopSign::Draw() {

	Texture2D diffuseTexture, specularTexture, normalTexture;
	diffuseTexture = ResourceManager::GetTexture("stop");
	specularTexture = ResourceManager::GetTexture("stop_specular");
	normalTexture = ResourceManager::GetTexture("stop_normal");
	this->RoadMarking->DrawSprite(diffuseTexture, specularTexture, normalTexture, glm::vec3(0.0f, 0.0f, -5.0f));

	// Draw Stop Line
	this->Line->DrawSprite(diffuseTexture, specularTexture, normalTexture, glm::vec3(0.0f, 0.0f, -5.0f));

	// Draw Model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(this->Position)); // translate it down so it's at the center of the scene
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));//rotation x = 0.0 degrees
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));	// it's a bit too big for our scene, so 
	ResourceManager::GetShader("shader").Use().SetMatrix4("model", model);
	this->model->Draw();

}