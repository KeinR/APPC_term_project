#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#define DEFAULT_YAW 60.0f
#define DEFAULT_PITCH 0
#define DEFAULT_SPEED 1
#define DEFAULT_M_SENSITIVITY 0.3
#define DEFAULT_FOV 80

Camera::Camera():
    worldUp(0.0f, 1.0f, 0.0f),
    position(0.0f, 0.0f, 0.0f),
    // front(0.0f, 0.0f, -1.0f), // Facing forward, down z
    // up(0.0f, 1.0f, 0.0f), // Oriented head facing "sky"
    yaw(DEFAULT_YAW),
    pitch(DEFAULT_PITCH),
    speed(DEFAULT_SPEED),
    mouseSensitivity(DEFAULT_M_SENSITIVITY),
    fov(DEFAULT_FOV)
{
    updateVectors();
}
glm::mat4 Camera::getView() {
    return glm::lookAt(position, position + front, up);
}
glm::mat4 Camera::getProj() {
    constexpr float VIEW_DIST = 1e10;
    // Second param is the viewport width/height
    return glm::perspective(glm::radians(fov), 1.0f, 0.1f, VIEW_DIST);
}
void Camera::processKey(dir key, float deltaTime) {
    float velocity = speed * deltaTime;
    switch (key) {
        case FORWARD:
            position += front * velocity;
            break;
        case BACKWARD:
            position -= front * velocity;
            break;
        case RIGHT:
            position += right * velocity;
            break;
        case LEFT:
            position -= right * velocity;
            break;
        case UP:
            position += up * velocity;
            break;
        case DOWN:
            position -= up * velocity;
            break;
    }
    // TODO: up and down keys
}
void Camera::processMouse(float xOffset, float yOffset) {
    yaw += xOffset * mouseSensitivity;
    pitch += yOffset * mouseSensitivity;

    // Prevent user from doing a backflip
    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateVectors();
}

void Camera::updateVectors() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // Get directions for up and down so that we can move correctly
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
