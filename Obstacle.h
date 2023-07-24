#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game_Object.h"

enum ObstacleType {
	STOP_SIGN
};

class Obstacle : public GameObject
{
public: 
	bool PassedObjective = false;
	bool infractionCommitted = false;

	Obstacle(ObstacleType obstacle, string const& path, Shader& shader, glm::vec3 pos);

};

#endif