#include "Sprite.h"

Sprite::Sprite(Shader& shader, std::vector<float> vertices)
{
    this->shader = shader;
    std::copy(vertices.begin(), vertices.end(), this->vertices);
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

    // plane VAO
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
}