#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

#include <iostream>

class Sprite
{
public:
    unsigned int diffuseMap, specularMap, normalMap;

    glm::vec3 pos1, pos2, pos3, pos4;
    glm::vec2 uv1, uv2, uv3, uv4;
    // Constructor (inits shaders/shapes)
    Sprite(Shader& shader, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3, glm::vec2 uv4);
    // Destructor
    ~Sprite();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D& diffuse_texture, Texture2D& specular_texture, Texture2D& normal_texture, glm::vec3 position);

    void renderQuad();

private:
    // Render state
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    Shader       shader;
    // Initializes and configures the quad's buffer and vertex attributes
};

#endif