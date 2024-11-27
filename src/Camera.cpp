#include "Camera.h"
#include "GLFW/glfw3.h"

Camera::Camera(glm::vec3 camera_position)
{
    Position = camera_position;
}

glm::mat4 Camera::GetViewMatrix()
{
    UpdateDirection();

    glm::mat4 view = glm::lookAt(Position, Position + Front, Up);

    return view;
}

void Camera::UpdateDirection()
{
    Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front.y = sin(glm::radians(Pitch));
    Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(Front);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    Speed = SpeedMultiplier * deltaTime;

    switch (direction)
    {
        case FOWARD:
          Position += Speed * Front;
          break;

        case BACKWARD:
          Position -= Speed * Front;
          break;

        case LEFT:
          Position -= glm::normalize(glm::cross(Front, Up)) * Speed;
          break;

        case RIGHT:
          Position += glm::normalize(glm::cross(Front, Up)) * Speed;
          break;
    }

    UpdateDirection();
}

void Camera::MouseMovementCallback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera)
    {
        camera->ProcessMouseMovement(xpos, ypos);
    }
}

void Camera::ProcessMouseMovement(double xpos, double ypos)
{
    if (FirstMouse)
    {
        LastX = xpos;
        LastY = ypos;
        FirstMouse = false;
    }

    float xoffset = xpos - LastX;
    float yoffset = ypos - LastY;

    LastX = xpos;
    LastY = ypos;

    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f) 
        Pitch = 89.0f;

    if (Pitch < -89.0f) 
        Pitch = -89.0f;
}

void Camera::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera)
    {
        camera->ProcessMouseScroll(xoffset, yoffset);
    }
}

void Camera::ProcessMouseScroll(double xoffset, double yoffset)
{
    (void) xoffset;

    Fov -= (float)yoffset;

    if (Fov < 1.0f)
      Fov = 1.0f;

    if (Fov > 45.0f)
      Fov = 45.0f;
}    

