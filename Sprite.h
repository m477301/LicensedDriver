#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

#include <array>
#include <vector>

class Sprite
{
public:
    unsigned int diffuseMap, specularMap, normalMap;
    // Constructor (inits shaders/shapes)
    Sprite(Shader& shader);
    // Destructor
    ~Sprite();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D& texture, glm::vec3 position, glm::vec3 size = glm::vec3(10.0f, 10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawSprite(Texture2D& diffuse_texture, Texture2D& specular_texture, glm::vec3 position, glm::vec3 size = glm::vec3(10.0f, 10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawSprite(Texture2D& diffuse_texture, Texture2D& specular_texture, Texture2D& normal_texture, glm::vec3 position, glm::vec3 size = glm::vec3(10.0f, 10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
    // Render state
    float vertices[48];
    Shader       shader;
    unsigned int VAO, VBO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif