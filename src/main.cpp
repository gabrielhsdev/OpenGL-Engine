// Various
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/Shader.h>
#include <engine/ecs.h> // All ECS logic and Shader
#include <engine/stb_image.h>

// clang-format off
float TRIANGLE_VERTICES[] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 1.0f, 0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 1.0f, 0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 1.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.5f,  0.0f, 1.0f
};
glm::vec3 CUBE_POSITIONS[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f)
};
// clang-format on

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

unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image data
    int width;
    int height;
    int nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip the image vertically
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data != nullptr) {
        GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture at " << path << '\n';
    }

    return textureID;
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
    if (!window) {
        std::cout << "Failed to create window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --- Shader (global for now, could later move into ResourceManager) ---
    Shader shader("shaders/shader.vert.glsl", "shaders/shader.frag.glsl");
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // --- ECS registry ---
    entt::registry registry;

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
    Scene prototypeScene{"Prototype",
                         // onLoad
                         [](entt::registry& reg) {
                             // Camera
                             auto camEnt = reg.create();
                             reg.emplace<Camera>(camEnt);
                             reg.emplace<CameraController>(camEnt);

                             // Input
                             auto inputEnt = reg.create();
                             reg.emplace<Input>(inputEnt);

                             // Mesh
                             unsigned int tex1 = loadTexture("textures/container.png");
                             unsigned int tex2 = loadTexture("textures/emogi.png");
                             MeshRenderer cubeMesh = MeshSystem::createCube(
                                 TRIANGLE_VERTICES, sizeof(TRIANGLE_VERTICES), tex1, tex2);

                             // Spawn cubes
                             for (auto& pos : CUBE_POSITIONS) {
                                 auto e = reg.create();
                                 Transform tf;
                                 tf.position = pos;
                                 reg.emplace<Transform>(e, tf);
                                 reg.emplace<MeshRenderer>(e, cubeMesh);
                             }
                         },
                         // onUnload
                         [](entt::registry& reg) {
                             reg.clear(); // remove all entities
                         },
                         // onUpdate
                         [](entt::registry& reg, float dt) {
                             static CameraSystem cameraSystem{reg};
                             static InputSystem inputSystem{reg};

                             cameraSystem.update(dt);
                             inputSystem.resetDeltas();
                         }};

    // --- SceneManager setup ---
    SceneManager sceneManager;
    sceneManager.addScene(prototypeScene);
    sceneManager.switchTo("Prototype", registry);

    // --- Main loop ---
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // update scene (input + camera are handled in onUpdate)
        sceneManager.update(registry, deltaTime);

        // find camera to render
        auto camView = registry.view<Camera>();
        if (!camView.empty()) {
            auto& cam = camView.get<Camera>(camView.front());
            RenderingSystem::renderScene(registry, shader, cam, SCR_WIDTH, SCR_HEIGHT);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}