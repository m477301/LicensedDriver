#include "Sprite.h"


Sprite::Sprite(Shader& shader)
{
    this->shader = shader;
    this->initRenderData();
}

Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

void Sprite::DrawSprite(Texture2D& texture, glm::vec3 position, glm::vec3 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    this->shader.Use();
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Sprite::initRenderData()
{
    // set up vertex data (and buffer(s)) and configure vertex 
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions            // normals         // texcoords
         3.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,  1.5f,  0.0f,
        -3.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -3.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         3.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,  1.5f,  0.0f,
        -3.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         3.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,  1.5f, 10.0f
    };
    // plane VAO
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
}