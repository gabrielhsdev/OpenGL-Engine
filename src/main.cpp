// Various
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Engine
#include <engine/Shader.h>
#include <engine/InputManager.h>
#include <engine/Camera.h>
#include <engine/CameraController.h>
#include <engine/stb_image.h>

// clang-format off
float RECTANGLE_VERTICES[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int RECTANGLE_INDICES[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};
float TRIANGLE_VERTICES[] = {
    /*
    //   x      y      z       r     g     b       texture S, texture T
        -0.9F,  0.9F,  0.0F,   1.0F, 0.0F, 0.0F,   1.0F, 1.0F, // top left (red)
        -0.7F,  0.9F,  0.0F,   0.0F, 1.0F, 0.0F,   0.0F, 1.0F, // top right (green)
        -0.8F,  0.7F,  0.0F,   0.0F, 0.0F, 1.0F,   0.0F, 0.0F, // bottom middle (blue)
    */
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
    (void)window; // Avoid unused parameter warning
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

void setupVAO(unsigned int& VAO) {
    glGenVertexArrays(1, &VAO); // Generate a VAO
    glBindVertexArray(VAO);
}

void setupVBO(unsigned int& VBO, float* vertices, size_t size) {
    // Generate a VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          (void*)(3 * sizeof(float))); // Disabled a warning on this line
    glEnableVertexAttribArray(1);

    // texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Do not unbind the VAO here when using EBO!
    // The VAO unbinding should be done after EBO setup
}

void setupTriangleWithoutEBO(unsigned int& VBO) {
    setupVBO(VBO, TRIANGLE_VERTICES, sizeof(TRIANGLE_VERTICES));

    // Unbind VAO since we're not using EBO
    glBindVertexArray(0);
}

void renderBackgroundColor(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers
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
    GLFWwindow* window = initWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
    if (window == nullptr || !initGlad()) {
        return -1;
    }

    // Setting up our main components
    Shader ourShader("shaders/shader.vert.glsl", "shaders/shader.frag.glsl");
    InputManager inputManager;
    CameraSettings cameraSettings = {
        glm::vec3(0.0f, 0.0f, 3.0f), // Position
        glm::vec3(0.0f, 1.0f, 0.0f), // World Up
        -90.0f,                      // Yaw
        0.0f,                        // Pitch
        45.0f                        // Fov
    };
    Camera camera(cameraSettings);
    CameraController cameraController(camera);

    // Initialize OpenGL options
    int fbWidth;
    int fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide cursor

    // Callbacks for input handling
    glfwSetWindowUserPointer(window, &inputManager);
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        if (!input)
            return;

        if (action == GLFW_PRESS) {
            input->KeyPress(key);
        } else if (action == GLFW_RELEASE) {
            input->KeyRelease(key);
        }
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto* input = static_cast<InputManager*>(glfwGetWindowUserPointer(win));
        if (!input)
            return;

        input->MouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
    });

    // Create separate VAOs for different objects
    unsigned int triangleVAO;
    unsigned int triangleVBO;

    // Textures
    unsigned int texture1 = loadTexture("textures/container.png");
    if (texture1 == 0) {
        std::cout << "Failed to load texture\n";
        return -1;
    }
    unsigned int texture2 = loadTexture("textures/emogi.png");
    if (texture2 == 0) {
        std::cout << "Failed to load texture2\n";
        return -1;
    }

    // Setup triangle (without EBO)
    setupVAO(triangleVAO);
    setupTriangleWithoutEBO(triangleVBO);

    // Use our shader
    ourShader.use(); // don't forget to activate the shader before setting uniforms!
    glUniform1i(glGetUniformLocation(ourShader.getShaderProgramID(), "texture1"),
                0);                  // set it manually
    ourShader.setInt("texture2", 1); // or with shader class

    // Coordinate Systems
    glm::mat4 model = glm::mat4(1.0f); // Initialize to identity matrix
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(cameraController.m_Camera.Fov),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, // Cast to float for proper division
                         0.1f,
                         100.0f);

    // Set transform matrices
    GLint modelLoc = glGetUniformLocation(ourShader.getShaderProgramID(), "model");
    GLint viewLoc = glGetUniformLocation(ourShader.getShaderProgramID(), "view");
    GLint projLoc = glGetUniformLocation(ourShader.getShaderProgramID(), "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Main loop
    float DELTATIME = 0.0f;     // Time between current frame and last frame
    float LAST_FRAME = 0.0f;    // Time of last frame
    float CURRENT_FRAME = 0.0f; // Current frame time

    while (glfwWindowShouldClose(window) == 0) {
        // Per frame time logic
        CURRENT_FRAME = glfwGetTime();
        DELTATIME = CURRENT_FRAME - LAST_FRAME;
        LAST_FRAME = CURRENT_FRAME;

        // Input
        if (inputManager.IsKeyPressed(GLFW_KEY_W)) {
            std::cout << "Moving forward\n";
            cameraController.ProcessKeyboard(FORWARD, DELTATIME);
        }
        if (inputManager.IsKeyPressed(GLFW_KEY_S)) {
            cameraController.ProcessKeyboard(BACKWARD, DELTATIME);
        }
        if (inputManager.IsKeyPressed(GLFW_KEY_A)) {
            cameraController.ProcessKeyboard(LEFT, DELTATIME);
        }
        if (inputManager.IsKeyPressed(GLFW_KEY_D)) {
            cameraController.ProcessKeyboard(RIGHT, DELTATIME);
        }
        cameraController.ProcessMouseMovement(inputManager.DeltaX, -inputManager.DeltaY);
        inputManager.Update();

        // Background color
        renderBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f); // Set background color

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Render triangle
        glBindVertexArray(triangleVAO);
        for (unsigned int i = 1; i < sizeof(CUBE_POSITIONS) / sizeof(CUBE_POSITIONS[0]); i++) {
            glm::mat4 model = glm::mat4(1.0f); // Reset model matrix for each cube
            model = glm::translate(model, CUBE_POSITIONS[i]);
            float angle = 20.0f * i * (float)glfwGetTime(); // Rotate each cube at different speeds
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Rotate camera around the origin
        view = glm::lookAt(cameraController.m_Camera.Position,
                           cameraController.m_Camera.Position + cameraController.m_Camera.Front,
                           cameraController.m_Camera.Up);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        projection = glm::perspective(glm::radians(cameraController.m_Camera.Fov),
                                      (float)SCR_WIDTH /
                                          (float)SCR_HEIGHT, // Cast to float for proper division
                                      0.1f,
                                      100.0f);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup all resources
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);
    glfwTerminate();
    return 0;
}
