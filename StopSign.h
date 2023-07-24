#ifndef STOPSIGN_H
#define STOPSIGN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Obstacle.h"
#include "Sprite.h"
#include "Resource_Manager.h"
#include "Texture.h"

class StopSign : public Obstacle
{
public: 
	Sprite* RoadMarking;
	Sprite* Line;

	StopSign(ObstacleType obstacle, string const& path, Shader& shader, Shader& modelShader, glm::vec3 pos);

	void Draw();
};

#endif