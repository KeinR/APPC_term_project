#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include <glm/glm.hpp>

class Camera {
    glm::vec3 worldUp; // Constant
    glm::vec3 position; // Where the camera is
    glm::vec3 front; // vector coming out of the camera's front
    glm::vec3 up; // vector coming out of the camera's top
    glm::vec3 right; // vector coming out of the camera's right
    float yaw; // around y axis
    float pitch; // around x axis
    float speed; // Camera speed
    float mouseSensitivity;
    float fov; // aka zoom

    void updateVectors();
public:

    // Camera movement directions
    enum dir {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    Camera();
    glm::mat4 getView();
    glm::mat4 getProj();
    // delta time is time last frame took
    void processKey(dir key, float deltaTime);
    // Offset from the last mouse position
    void processMouse(float xOffset, float yOffset);
};

#endif
