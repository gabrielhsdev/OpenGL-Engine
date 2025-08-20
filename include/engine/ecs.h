#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <iostream>
#include <glad/glad.h>
#include <entt/entt.hpp>
#include "engine/Shader.h"

// --- Components ---
struct Transform {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f}; // Euler angles (degrees)
    glm::vec3 scale{1.0f};
};

struct Camera {
    glm::vec3 position{0.0f, 0.0f, 20.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
    float yaw{-90.0f};
    float pitch{0.0f};
    float fov{45.0f};
};

struct CameraController {
    float movementSpeed{2.5f};
    float mouseSensitivity{0.1f};
};

struct Input {
    std::unordered_map<int, bool> keys;
    float mouseX{0.0f}, mouseY{0.0f};
    float deltaX{0.0f}, deltaY{0.0f};
    bool firstMouse{true};
};

// This is for simple mesh rendering
struct MeshRenderer {
    unsigned int VAO{0};
    unsigned int VBO{0};
    unsigned int vertexCount{0};
    unsigned int texture1{0};
    unsigned int texture2{0};
};

struct Scene {
    std::string name;

    // Functions you provide when defining a scene
    std::function<void(entt::registry&)> onLoad;
    std::function<void(entt::registry&)> onUnload;
    std::function<void(entt::registry&, float)> onUpdate;
};

// TODO: This is for more complex models with multiple meshes, add assimp. Dont remove this for now
struct ModelMesh {
    unsigned int VAO{0};
    unsigned int VBO{0};
    unsigned int EBO{0};
    unsigned int indexCount{0};
    unsigned int texture1{0};
    unsigned int texture2{0};
};

// TODO: This is for more complex models with multiple meshes, add assimp. Dont remove this for now
struct ModelRenderer {
    std::vector<ModelMesh> meshes;
};

// --- Helper Enums ---
enum class CameraMovement : std::uint8_t { FORWARD, BACKWARD, LEFT, RIGHT };

// --- Camera System ---
class CameraSystem {
  public:
    static void updateCameraVectors(Camera& cam) {
        glm::vec3 front;
        front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
        front.y = sin(glm::radians(cam.pitch));
        front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
        cam.front = glm::normalize(front);
        cam.right = glm::normalize(glm::cross(cam.front, cam.worldUp));
        cam.up = glm::normalize(glm::cross(cam.right, cam.front));
    }

    static glm::mat4 getViewMatrix(const Camera& cam) {
        return glm::lookAt(cam.position, cam.position + cam.front, cam.up);
    }

    static void processKeyboard(Camera& cam,
                                const CameraController& ctrl,
                                CameraMovement dir,
                                float deltaTime) {
        float velocity = ctrl.movementSpeed * deltaTime;
        switch (dir) {
        case CameraMovement::FORWARD:
            cam.position += cam.front * velocity;
            break;
        case CameraMovement::BACKWARD:
            cam.position -= cam.front * velocity;
            break;
        case CameraMovement::LEFT:
            cam.position -= cam.right * velocity;
            break;
        case CameraMovement::RIGHT:
            cam.position += cam.right * velocity;
            break;
        }
        updateCameraVectors(cam);
    }

    static void processMouse(Camera& cam,
                             const CameraController& ctrl,
                             float xoffset,
                             float yoffset,
                             bool constrainPitch = true) {
        xoffset *= ctrl.mouseSensitivity;
        yoffset *= ctrl.mouseSensitivity;

        cam.yaw += xoffset;
        cam.pitch += yoffset;

        if (constrainPitch)
            cam.pitch = std::clamp(cam.pitch, -89.0f, 89.0f);

        updateCameraVectors(cam);
    }
};

// --- Input System ---
class InputSystem {
  public:
    static void handleMouse(Input& input, double xpos, double ypos) {
        if (input.firstMouse) {
            input.mouseX = xpos;
            input.mouseY = ypos;
            input.firstMouse = false;
        }

        input.deltaX = xpos - input.mouseX;
        input.deltaY = ypos - input.mouseY;
        input.mouseX = xpos;
        input.mouseY = ypos;
    }

    static void resetDeltas(Input& input) {
        input.deltaX = 0.0f;
        input.deltaY = 0.0f;
    }
};

// --- Mesh System ---
class MeshSystem {
  public:
    static MeshRenderer createCube(float* vertices,
                                   size_t vertSize,
                                   unsigned int texture1 = 0,
                                   unsigned int texture2 = 0) {
        MeshRenderer mesh;
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);

        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        mesh.vertexCount = static_cast<unsigned int>(vertSize / (8 * sizeof(float)));
        mesh.texture1 = texture1;
        mesh.texture2 = texture2;
        return mesh;
    }
};

// --- Rendering System ---
class RenderingSystem {
  public:
    static void renderEntity(const Transform& tf, const MeshRenderer& mesh, Shader& shader) {
        if (mesh.texture1 != 0u) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.texture1);
        }
        if (mesh.texture2 != 0u) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mesh.texture2);
        }

        glBindVertexArray(mesh.VAO);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, tf.position);
        model = glm::rotate(model, glm::radians(tf.rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(tf.rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(tf.rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, tf.scale);

        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);

        glBindVertexArray(0);
    }

    static void renderScene(entt::registry& registry,
                            Shader& shader,
                            const Camera& cam,
                            int width,
                            int height) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = CameraSystem::getViewMatrix(cam);
        glm::mat4 projection =
            glm::perspective(glm::radians(cam.fov), float(width) / height, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        auto viewMesh = registry.view<Transform, MeshRenderer>();
        viewMesh.each([&](auto& tf, auto& mesh) { renderEntity(tf, mesh, shader); });
    }
};

// --- Scene System ---
class SceneManager {
  public:
    void addScene(const Scene& scene) {
        scenes[scene.name] = scene;
    }

    void switchTo(const std::string& name, entt::registry& registry) {
        if (!current.empty() && scenes[current].onUnload) {
            scenes[current].onUnload(registry);
        }
        current = name;
        if (scenes[current].onLoad) {
            scenes[current].onLoad(registry);
        }
    }

    void update(entt::registry& registry, float dt) {
        if (!current.empty() && scenes[current].onUpdate) {
            scenes[current].onUpdate(registry, dt);
        }
    }

  private:
    std::unordered_map<std::string, Scene> scenes;
    std::string current;
};