#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/stb_image.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>
#include <engine/Shader.h>
#include <engine/Texture.h>

// --- Components ---
struct Transform {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f}; // Euler angles (degrees)
    glm::vec3 scale{1.0f};
};

struct Camera {
    glm::vec3 position{0.0f, 0.0f, 5.0f};
    glm::vec3 front{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    glm::vec3 right{1.0f, 0.0f, 0.0f};
    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
    float yaw{-90.0f};
    float pitch{0.0f};
    float fov{45.0f};

    void realocVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
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

struct DeltaTime {
    float deltaTime{0.0f}; // Time between current frame and last frame
};

// This is for simple mesh rendering
struct MeshRenderer {
    unsigned int VAO{0};
    unsigned int VBO{0};
    unsigned int vertexCount{0};
    unsigned int texture1{0};
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
};

// TODO: This is for more complex models with multiple meshes, add assimp. Dont remove this for now
struct ModelRenderer {
    std::vector<ModelMesh> meshes;
};

// --- Helper Enums ---
enum class CameraMovement : std::uint8_t { FORWARD, BACKWARD, LEFT, RIGHT };

// --- Camera System ---
class CameraSystem { // We will loop the registry instead of having to pass the camera as a prop
  public:
    CameraSystem(entt::registry& reg) : registry(reg) {} ///< Constructor takes a registry reference

    void update(float deltaTime) { ///< Update method to process camera input
        auto view = registry.view<Camera, CameraController>();

        for (auto entity : view) {
            auto& cam = view.get<Camera>(entity);
            auto& ctrl = view.get<CameraController>(entity);
            auto& input = registry.view<Input>().get<Input>(registry.view<Input>().front());

            if (input.keys[GLFW_KEY_W]) {
                processKeyboard(cam, ctrl, CameraMovement::FORWARD, deltaTime);
            }
            if (input.keys[GLFW_KEY_S]) {
                processKeyboard(cam, ctrl, CameraMovement::BACKWARD, deltaTime);
            }

            if (input.keys[GLFW_KEY_A]) {
                processKeyboard(cam, ctrl, CameraMovement::LEFT, deltaTime);
            }

            if (input.keys[GLFW_KEY_D]) {
                processKeyboard(cam, ctrl, CameraMovement::RIGHT, deltaTime);
            }

            processMouse(cam, ctrl, input.deltaX, -input.deltaY);
            cam.realocVectors(); // Update camera vectors after processing input
        }
    }

  private:
    entt::registry& registry; ///< Reference to the registry for accessing components and loops
                              ///< through them ( finds cameras )

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

        if (constrainPitch) {
            cam.pitch = std::clamp(cam.pitch, -89.0f, 89.0f);
        }
    }
};

// --- Input System ---
class InputSystem {
  public:
    InputSystem(entt::registry& reg) : registry(reg) {}

    void resetDeltas() {
        auto view = registry.view<Input>();
        for (auto e : view) {
            auto& input = view.get<Input>(e);
            input.deltaX = 0.0f;
            input.deltaY = 0.0f;
        }
    }

  private:
    entt::registry& registry;
};

// --- Mesh System ---
class MeshSystem {
  public:
    static MeshRenderer createCube(float* vertices, size_t vertSize, unsigned int texture1 = 0) {
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
        return mesh;
    }
};

// --- Rendering System ---
class RenderingSystem {
  public:
    RenderingSystem(entt::registry& reg) : registry(reg) {}

    void update(Shader& shader, const Camera& cam, int width, int height) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cam.position, cam.position + cam.front, cam.up);
        glm::mat4 projection =
            glm::perspective(glm::radians(cam.fov), float(width) / height, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        auto viewMesh = registry.view<Transform, MeshRenderer>();
        viewMesh.each([&](auto& tf, auto& mesh) { renderEntity(tf, mesh, shader); });
    }

  private:
    entt::registry& registry;

    static void renderEntity(const Transform& transform, const MeshRenderer& mesh, Shader& shader) {
        if (mesh.texture1 != 0u) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.texture1);
        }

        glBindVertexArray(mesh.VAO);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, transform.scale);

        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);

        glBindVertexArray(0);
    }
};

// --- Scene Manager ---
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
        } else {
            std::cout << "No active scene to update.\n";
        }
    }

  private:
    std::unordered_map<std::string, Scene> scenes;
    std::string current;
};

// --- Resource Manager ---
template <typename Resource> class ResourceManager {
  public:
    using Loader = std::function<std::unique_ptr<Resource>(const std::string&)>;
    using Deleter = std::function<void(Resource&)>;

    template <typename LoaderFn>
    static Resource& load(const std::string& name, const std::string& path, LoaderFn loader) {
        resources[name] = loader(path);
        return *resources[name];
    }

    static Resource& get(const std::string& name) {
        return *resources.at(name);
    }

    static void clear(const std::string& name) {
        resources.erase(name);
    }

    static void clearAll(Deleter deleter = nullptr) {
        if (deleter) {
            for (auto& [_, res] : resources) {
                deleter(*res);
            }
        }
        resources.clear();
    }

  private:
    static inline std::unordered_map<std::string, std::unique_ptr<Resource>> resources;
};