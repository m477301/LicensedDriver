#include "Player.h"

Player::Player(string const& path, Shader& shader, glm::vec3 pos)
	: Position(0.0f, 0.0f, 0.0f), Size(10.0f, 10.0f, 1.0f), shader(shader) {
    this->MovementSpeed = CAR_SPEED;
    model = Model(path, shader);

    ourEntity = Entity(model.meshes[0]);
    ourEntity.transform.setLocalPosition({ 0, 0, 0 });
    const float scale = 0.01f;
    ourEntity.transform.setLocalScale({ scale, scale, scale });

    {
        Entity* lastEntity = &ourEntity;
        Entity* childEntity = &ourEntity;

        for (int i = 1; i < model.meshes.size(); i++) {
            lastEntity->addChild(model.meshes[i]);
            childEntity = lastEntity->children.back().get();

            switch (i) {
            case 2:
                childEntity->transform.setLocalPosition({ 41.456f, 10.616f, -48.042f });
                break;
            case 3:
                childEntity->transform.setLocalPosition({ -41.467f, 10.63f, -48.031f });
                break;
            case 4:
                childEntity->transform.setLocalPosition({ -41.4711f, 10.628f, 51.9718f });
                break;
            case 5:
                childEntity->transform.setLocalPosition({ 41.465f, 10.63f, 51.969f });
                break;
            default:
                childEntity->transform.setLocalPosition({ 0.0f, 0.0f, 0.0f });
                break;
            }
            childEntity->transform.setLocalRotation({ 0.0f, 0.0f, 0.0f });
            childEntity->transform.setLocalScale({ 1.f, 1.f, 1.f });
        }
    }
    ourEntity.updateSelfAndChild();
}

void Player::update(float deltaTime) {
    this->changeInPosition = this->Position - this->PreviousPosition;
    this->PreviousPosition = this->Position;
    this->initialVelocity = this->Velocity;
    this->Velocity = ((this->changeInPosition) / deltaTime);
    this->changeInVelocity = this->Velocity - this->initialVelocity;
    this->Acceleration = CAR_ACCELERATION * (this->changeInVelocity / deltaTime);
}

void Player::Draw(float deltaTime) {

    //draw our scene graph
    Entity* lastEntity = &ourEntity;

        ResourceManager::GetShader("shader").Use().SetMatrix4("model", lastEntity->transform.getModelMatrix());
        lastEntity->pMesh->Draw(this->shader);

        int i = 0;
        // Go through the children
        for (auto const& a : lastEntity->children) {
            glm::vec3 rotation = glm::vec3(a->transform.getLocalRotation().x, 0.f, a->transform.getLocalRotation().z);
            if (i >= 1) {
                switch (forwardDirection) {
                case 1:
                    rotation += glm::vec3(50.0f * deltaTime, 0.f, 0.f);
                    break;
                case 2:
                    rotation += glm::vec3(-(50.0f * deltaTime), 0.f, 0.f);
                    break;
                default:
                    break;
                }
                if (i >= 3) {
                    float turnAngle = a->transform.getLocalRotation().y;
                    if (turnAngle > 45.0f) {
                        turnAngle = 45.0f;
                    }
                    else if (turnAngle < -45.0f) {
                        turnAngle = -45.0f;
                    }

                    switch (rotationDirection) {
                    case 0:
                        turnAngle = 0;
                        break;
                    case 1:
                        turnAngle += 1.0f;
                        break;
                    case 2:
                        turnAngle -= 1.0f;
                        break;
                    default:
                        break;
                    }
                    rotation += glm::vec3(0.f, turnAngle, 0.f);
                }
                a->transform.setLocalRotation(rotation);
            }

            ResourceManager::GetShader("shader").Use().SetMatrix4("model", a->transform.getModelMatrix());

            a->pMesh->Draw(this->shader);
            i++;
        }
    ourEntity.transform.setLocalPosition(this->Position);
    ourEntity.transform.setLocalRotation({ 0.0f, this->Rotation, 0.0f });
    ourEntity.updateSelfAndChild();
}

void Player::move(Movement direction, float deltaTime) {
    float speed = MovementSpeed * deltaTime;
    // Velocity in all axes is
    if (direction == FORWARD) {
        this->Position += carFront * (speed);
        forwardDirection = 1;
    }
    if (direction == BACKWARD) {
        this->Position -= carFront * (speed);
        forwardDirection = 2;
    }
    if (direction == LEFT) {
        this->Rotation += this->RotationSpeed;
        glm::vec3 front;
        front.x = sin(glm::radians(Rotation));
        front.y = 0.0f;
        front.z = cos(glm::radians(Rotation));
        this->carFront = glm::normalize(front);
        rotationDirection = 1;
    }
    if (direction == RIGHT) {
        this->Rotation -= this->RotationSpeed;
        glm::vec3 front;
        front.x = sin(glm::radians(Rotation));
        front.y = 0.0f;
        front.z = cos(glm::radians(Rotation));
        this->carFront = glm::normalize(front);
        rotationDirection = 2;
    }
}