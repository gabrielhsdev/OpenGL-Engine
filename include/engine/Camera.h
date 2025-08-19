#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>

struct CameraSettings {
    glm::vec3 Position; ///< Camera position in world space
    glm::vec3 WorldUp;  ///< World up vector, typically (0.0f, 1.0f, 0.0f)
    float Yaw;          ///< Yaw angle, rotation around the vertical axis
    float Pitch;        ///< Pitch angle, rotation around the horizontal axis
    float Fov;          ///< Field of view, used for perspective projection
};

class Camera {
  public:
    glm::vec3 Position; ///< Camera position in world space
    glm::vec3 Front;    ///< Camera front vector, pointing in the direction the camera is facing
    glm::vec3 Up;       ///< Camera up vector, pointing upwards
    glm::vec3 Right;    ///< Camera right vector, perpendicular to the front and up vectors
    glm::vec3 WorldUp;  ///< World up vector, typically (0.0f, 1.0f, 0.0f)

    float Yaw;   ///< Yaw angle, rotation around the vertical axis
    float Pitch; ///< Pitch angle, rotation around the horizontal axis
    float Fov;   ///< Field of view, used for perspective projection

    Camera(const CameraSettings& settings)
        : Position(settings.Position), WorldUp(settings.WorldUp), Yaw(settings.Yaw),
          Pitch(settings.Pitch), Fov(settings.Fov) {
        UpdateCameraVectors();
    }

    // Get the view matrix for the camera
    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void UpdateCameraVectors() {
        // Calculate the new front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        // Re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(
            Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the
                              // more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif
