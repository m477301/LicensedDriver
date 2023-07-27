#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Sprite.h"
#include "Model.h"
#include "Game.h"
#include "Entity.h"


const float CAR_SPEED = 2.5f;
const float CAR_ACCELERATION = 1.5;

class Player
{
public:
	glm::vec3   Position, Size;
	glm::vec3   Velocity, Acceleration, Speed;
	glm::vec3   initialVelocity, PreviousPosition;
	glm::vec3   changeInPosition, changeInVelocity;
	float       Rotation;
	float       RotationSpeed = 0.5;
	float       MovementSpeed;

	int forwardDirection = 0;
	int rotationDirection = 0;

	Model model;
	Entity ourEntity;

	Shader shader;

	glm::vec3 carFront = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

	Player(string const& path, Shader& Shader, glm::vec3 pos);

	void update(float deltaTime);

	void move(Movement direction, float deltaTime);

	void Draw(float deltaTime);
};

#endif
