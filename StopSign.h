#ifndef STOPSIGN_H
#define STOPSIGN_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Obstacle.h"
#include "Sprite.h"
#include "Resource_Manager.h"
#include "Texture.h"

class StopSign : public Obstacle
{
public: 
	Sprite* RoadMarking;

	StopSign(ObstacleType obstacle, string const& path, Shader& shader, glm::vec3 pos);

	void Render();
};

#endif