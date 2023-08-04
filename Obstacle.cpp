#include "Obstacle.h";

Obstacle::Obstacle(ObstacleType obstacle, string const& path, Shader& shader, glm::vec3 pos) : Position(pos), model(new Model(path, shader)) { }
