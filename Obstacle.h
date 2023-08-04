#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

enum ObstacleType {
	STOP_SIGN
};

class Obstacle
{
public: 
	bool PassedObjective = false;
	bool correctManoeuvre = false;

	glm::vec3   Position, Size;
	Model* model;

	Obstacle(ObstacleType obstacle, string const& path, Shader& shader, glm::vec3 pos);

};

#endif