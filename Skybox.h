#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

#include "Resource_Manager.h"

class Skybox
{
public:

	unsigned int skyboxVAO, cubemapTexture;

	Skybox();

	void Draw();

	unsigned int loadCubemap(std:: vector<std::string> faces);
};

#endif
