#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/Camera.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <iostream>

enum Camera_Movement : std::uint8_t { FORWARD, BACKWARD, LEFT, RIGHT };
struct CameraSetting {
    float speed;
    float sensitivity;
};

class CameraController {
  public:
    Camera& m_Camera;
    float MovementSpeed;
    float MouseSensitivity;

    CameraController(Camera& camera, CameraSetting settings = {2.5f, 0.1f})
        : m_Camera(camera), MovementSpeed(settings.speed), MouseSensitivity(settings.sensitivity) {}

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD) {
            m_Camera.Position += m_Camera.Front * velocity;
        }
        if (direction == BACKWARD) {
            m_Camera.Position -= m_Camera.Front * velocity;
        }
        if (direction == LEFT) {
            m_Camera.Position -= glm::normalize(glm::cross(m_Camera.Front, m_Camera.Up)) * velocity;
        }
        if (direction == RIGHT) {
            m_Camera.Position += glm::normalize(glm::cross(m_Camera.Front, m_Camera.Up)) * velocity;
        }
        m_Camera.UpdateCameraVectors(); // Update camera vectors after movement
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        // Lets debug our offsets
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        m_Camera.Yaw += xoffset;
        m_Camera.Pitch += yoffset;
        // Debugging output
        std::cout << "Yaw: " << m_Camera.Yaw << ", Pitch: " << m_Camera.Pitch << std::endl;

        if (constrainPitch) {
            m_Camera.Pitch = std::min(m_Camera.Pitch, 89.0f);
            m_Camera.Pitch = std::max(m_Camera.Pitch, -89.0f);
        }

        m_Camera.UpdateCameraVectors();
    }
};

#endif // CAMERA_CONTROLLER_H
