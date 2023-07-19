#include "Camera.h"
#include <iostream>

// constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 1.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    std::cout << "First Constructor Call" << std::endl;
    updateCameraVectors();
}
// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 1.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    std::cout << "Second Constructor Call" << std::endl;
    //updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix(glm::vec3 carPos, float carYaw)
{ 
    if (!this->view) {
        this->Position = carPos
            + (
                glm::vec3(3.0f, 1.0f, 3.0f)
                *
                (
                    glm::vec3(
                        -sin(glm::radians(carYaw)),
                        1.0f,
                        -cos(glm::radians(carYaw))
                    )
                    )
                );

        return glm::lookAt(this->Position, carPos, this->Up);
    }

    float zDis = -0.25f * cos(glm::radians(carYaw)) - (0.2f) * sin(glm::radians(carYaw));
    float xDis = 0.2f * cos(glm::radians(carYaw)) + (-0.25f) * sin(glm::radians(carYaw));
    this->Position = carPos + glm::vec3(
        xDis,
        0.6f, 
        zDis
        );

    this->Front = glm::vec3(
        (sin(glm::radians(carYaw) + glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch))),
        0.0f, 
        (cos(glm::radians(carYaw) + glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch)))
    );

    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainYaw)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw -= xoffset;
    Pitch += yoffset;


    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainYaw)
    {
        if (Yaw > 89.0f)
            Yaw = 89.0f;
        if (Yaw < -89.0f)
            Yaw = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::changeViewPosition() {
    this->view = !this->view;
    this->view ? Zoom = 80.0f : Zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.z = cos(glm::radians(this->Yaw)) * cos(glm::radians(Pitch));
    front.y = 0.0f;
    front.x = sin(glm::radians(this->Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

