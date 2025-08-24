#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/Shader.h>
#include <engine/ecs.h>
#include <engine/stb_image.h>
#include <engine/simpleMeshes.h>
#include <engine/DeltaTime.h>

// TODO: Improve our onUpdate ? Maybe we can have for example, the shader and camera inside the
// SceneManager loop Since we will always have to loop though them, right ? Easy/Medium

// TODO: Create some kind of model-loader using assimp so we dont need to use the basic shape
// factory anymore, after doing that we can finally look into removing the basic shapes and its
// factory. We may have default cube / simple shapes FBX files then / Hard !

// TODO: Refactor the entire code so we can split into more files and start a better engine kick /
// Easy !

// TODO: Implement transform animation system

// TODO: Implement fbx animation system

// TODO: Implement lights / shadows

// TODO: Implement glass

// TODO: Implement sound ?

// timing
float DELTA_TIME = 0.0f; // time between current frame and last frame
float LAST_FRAME = 0.0f;
int SCR_WIDTH = 1280; // Window width
int SCR_HEIGHT = 720; // Window height

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* initWindow(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);      // macOS forward compat
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE); // macOS retina

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    return window;
}

bool initGlad() {
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        std::cout << "Failed to initialize GLAD" << "\n";
        return false;
    }
    return true;
}

int main() {
    // --- Initialize GLFW and GLAD ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL ECS", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    // lOCK IN MOUSE
    glfwSetCursorPos(window, SCR_WIDTH / 2.0, SCR_HEIGHT / 2.0);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --- ECS registry ---
    entt::registry registry;

    // Register DeltaTime as a singleton component by providing an instance
    registry.ctx().emplace<DeltaTime>();

    // --- GLFW callbacks for input ---
    glfwSetWindowUserPointer(window, &registry);
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int sc, int action, int mods) {
        auto* reg = static_cast<entt::registry*>(glfwGetWindowUserPointer(win));
        if (auto view = reg->view<Input>(); !view.empty()) {
            auto& input = reg->get<Input>(view.front());
            input.keys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
        }
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto* reg = static_cast<entt::registry*>(glfwGetWindowUserPointer(win));
        if (!reg)
            return;

        auto view = reg->view<Input>();
        if (view.empty())
            return;

        auto& input = view.get<Input>(view.front());
        input.deltaX += static_cast<float>(xpos - input.mouseX);
        input.deltaY += static_cast<float>(ypos - input.mouseY);
        input.mouseX = static_cast<float>(xpos);
        input.mouseY = static_cast<float>(ypos);
    });

    // --- Create our scene ---
    Scene prototypeScene{
        "Prototype",
        // onLoad
        [](entt::registry& reg) {
            // Camera
            auto camEnt = reg.create();
            reg.emplace<Camera>(camEnt);
            reg.emplace<CameraController>(camEnt);

            // Shaders
            auto& shader = ResourceManager<Shader>::load(
                "basic", "shaders/shader.vert.glsl;shaders/shader.frag.glsl", shaderLoader);
            shader.use();
            shader.setInt("texture1", 0);

            // Input
            auto inputEnt = reg.create();
            reg.emplace<Input>(inputEnt);

            // Mesh
            auto& texture =
                ResourceManager<Texture>::load("brick", "textures/bricks.png", textureLoader);

            MeshRenderer cubeMesh =
                MeshSystem::createCube(CUBE_VERTICES, sizeof(CUBE_VERTICES), texture.getId());

            // Spawn cubes
            for (auto& pos : CUBE_POSITIONS) {
                auto e = reg.create();
                Transform transform;
                transform.position = pos;
                reg.emplace<Transform>(e, transform);
                reg.emplace<MeshRenderer>(e, cubeMesh);
            }
        },
        // onUnload
        [](entt::registry& reg) {
            reg.clear();                          // remove all entities
            ResourceManager<Shader>::clearAll();  // shaders free automatically
            ResourceManager<Texture>::clearAll(); // textures deleted in ~Texture
        },
        // onUpdate
        [](entt::registry& reg) {
            // Register our systems
            static RenderingSystem renderingSystem{reg};
            static CameraSystem cameraSystem{reg};
            static InputSystem inputSystem{reg};

            // Get Needed Instances
            auto& ShaderInstance = ResourceManager<Shader>::get("basic");
            auto& dtManager = reg.ctx().get<DeltaTime>();
            auto camView = reg.view<Camera>();

            cameraSystem.update(dtManager.getTime().deltaTime);
            inputSystem.resetDeltas();
            if (!camView.empty()) {
                auto& cam = camView.get<Camera>(camView.front());
                renderingSystem.update(ShaderInstance, cam, SCR_WIDTH, SCR_HEIGHT);
            }
        }};

    // --- SceneManager setup ---
    SceneManager sceneManager;
    sceneManager.addScene(prototypeScene);
    sceneManager.switchTo("Prototype", registry);

    // --- Main loop ---
    auto& dtManager = registry.ctx().get<DeltaTime>();
    while (glfwWindowShouldClose(window) == 0) {
        dtManager.calculateDeltaTime();

        // update scene (input + camera + rendering are handled in onUpdate)
        sceneManager.update(registry);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}