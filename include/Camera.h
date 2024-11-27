#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement { FOWARD, BACKWARD, LEFT, RIGHT};

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up = glm::vec3(0.0f, 9.0f, 0.0f);

    float Fov = 45.0f;
    float Speed = 0.0f;
    float SpeedMultiplier = 3.5f;
    float Sensitivity = 0.0f;

    float Yaw = 0.0f;
    float Pitch = 0.0f;

    float LastX = 400, LastY = 300;
    bool FirstMouse = true;


    Camera(glm::vec3 position);
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    static void MouseMovementCallback(GLFWwindow* window, double xoffset, double yoffset);
    void ProcessMouseMovement(double xoffset, double yoffset);

    static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void ProcessMouseScroll(double xoffset, double yoffset);

    void UpdateDirection();
};

#endif
