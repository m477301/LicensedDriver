#include "StopSign.h"

#include <iostream>

StopSign::StopSign(ObstacleType obstacle, string const& path, Shader& shader, glm::vec3 pos)
	:
	Obstacle(obstacle, path, shader, pos) 
{ 
	RoadMarking = new Sprite
	(
		shader,
		(pos + glm::vec3(-0.5f, 0.01f, 0.0f)),
		(pos + glm::vec3(-0.5f, 0.01f, 1.0f)),
		(pos + glm::vec3(-2.0f, 0.01f, 1.0f)),
		(pos + glm::vec3(-2.0f, 0.01f, 0.0f))
	);
}

void StopSign::Render() {

	Texture2D diffuseTexture, specularTexture, normalTexture;
	diffuseTexture = ResourceManager::GetTexture("stop");
	specularTexture = ResourceManager::GetTexture("stop_specular");
	normalTexture = ResourceManager::GetTexture("stop_normal");
	this->RoadMarking->DrawSprite(diffuseTexture, specularTexture, normalTexture, glm::vec3(0.0f, 0.0f, -5.0f));

	// Draw Stop Line

	// Draw Model

}