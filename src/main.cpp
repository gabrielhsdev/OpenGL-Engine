#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <engine/Shader.h>
#include <engine/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO: Fix vscode play button to build and run the project

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

/**
 * @brief Callback function to adjust the viewport when the window size changes.
 * @param window A pointer to the GLFWwindow that received the event.
 * @param width The new width, in pixels, of the window.
 * @param height The new height, in pixels, of the window.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void)window; // Avoid unused parameter warning
    glViewport(0, 0, width, height);
}

/**
 * @brief Processes user input, specifically checking for the ESCAPE key to close the window.
 * @param window A pointer to the GLFWwindow to check for input.
 */
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}

/**
 * @brief Initializes GLFW, configures window hints, and creates a GLFW window.
 * @param width The desired width of the window.
 * @param height The desired height of the window.
 * @param title The title of the window.
 * @return A pointer to the created GLFWwindow, or nullptr if window creation fails.
 */
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

/**
 * @brief Initializes GLAD to load OpenGL function pointers.
 * @return True if GLAD initialization is successful, false otherwise.
 */
bool initGlad() {
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        std::cout << "Failed to initialize GLAD" << "\n";
        return false;
    }
    return true;
}

/**
 * @brief Sets up our VAO
 * @param VAO The ID of the Vertex Array Object to set up.
 */
void setupVAO(unsigned int& VAO) {
    glGenVertexArrays(1, &VAO); // Generate a VAO
    glBindVertexArray(VAO);
}

/**
 * @brief Sets up our VBO, can be used for rendering a triangle without EBO or with EBO
 * @param VBO The ID of the Vertex Buffer Object to set up.
 * @param vertices Pointer to the vertex data array.
 * @param size The size of the vertex data array in bytes.
 */
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

/**
 * @brief Sets up a rectangle using an Element Buffer Object (EBO).
 * @param VAO The ID of the Vertex Array Object to set up.
 * @param VBO The ID of the Vertex Buffer Object to set up.
 * @param EBO The ID of the Element Buffer Object to set up.
 */
void setupRectangleWithEBO(unsigned int& VBO, unsigned int& EBO) {
    // Setup VBO ( we need vertices for that )
    setupVBO(VBO, RECTANGLE_VERTICES, sizeof(RECTANGLE_VERTICES));

    // Create EBO (must be done while VAO is still bound)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDICES), RECTANGLE_INDICES, GL_STATIC_DRAW);

    // Now unbind the VAO after both VBO and EBO are set up
    glBindVertexArray(0);
}

/**
 * @brief Sets up a triangle without using an Element Buffer Object (EBO).
 * @param VAO The ID of the Vertex Array Object to set up.
 * @param VBO The ID of the Vertex Buffer Object to set up.
 */
void setupTriangleWithoutEBO(unsigned int& VBO) {
    setupVBO(VBO, TRIANGLE_VERTICES, sizeof(TRIANGLE_VERTICES));

    // Unbind VAO since we're not using EBO
    glBindVertexArray(0);
}

/**
 * @brief Renders the background color using glClearColor and glClear.
 * @param red Red component of the background color (0.0 to 1.0).
 * @param green Green component of the background color (0.0 to 1.0).
 * @param blue Blue component of the background color (0.0 to 1.0).
 * @param alpha Alpha component of the background color (0.0 to 1.0).
 */
void renderBackgroundColor(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers
}

/**
 * @brief Loads a texture from a file and generates mipmaps.
 * @param path The file path to the texture image.
 * @return The OpenGL texture ID, or 0 if loading fails.
 */
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

glm::mat4 getTransformMatrix(float angle, glm::vec3 translation, glm::vec3 scale) {
    glm::mat4 transform = glm::mat4(1.0f);              // Initialize to identity matrix
    transform = glm::translate(transform, translation); // Apply translation
    transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // Apply rotation
    transform = glm::scale(transform, scale);                               // Apply scaling
    return transform;
}

/**
 * @brief Main function of the OpenGL application.
 * Initializes GLFW, GLAD, sets up the window, compiles and links shaders, prepares vertex data,
 * enters the main rendering loop, and cleans up resources upon exit.
 * @return 0 if the program executes successfully, -1 otherwise.
 */
int main() {
    GLFWwindow* window = initWindow(1280, 720, "LearnOpenGL");
    if (window == nullptr) {
        return -1;
    }

    if (!initGlad()) {
        return -1;
    }

    // Get actual framebuffer size (important for high-DPI)
    int fbWidth;
    int fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("shaders/shader.vert.glsl", "shaders/shader.frag.glsl");

    // Create separate VAOs for different objects
    unsigned int triangleVAO;
    unsigned int rectangleVAO;
    unsigned int triangleVBO;
    unsigned int rectangleVBO;
    unsigned int rectangleEBO;

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

    // Setup rectangle (with EBO)
    setupVAO(rectangleVAO);
    setupRectangleWithEBO(rectangleVBO, rectangleEBO);

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
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Main loop
    while (glfwWindowShouldClose(window) == 0) {
        // Preparaion for rendering
        processInput(window);                          // Check for user input
        renderBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f); // Set background color

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Set transform matrices
        GLint modelLoc = glGetUniformLocation(ourShader.getShaderProgramID(), "model");
        GLint viewLoc = glGetUniformLocation(ourShader.getShaderProgramID(), "view");
        GLint projLoc = glGetUniformLocation(ourShader.getShaderProgramID(), "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

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

        // Render rectangle (uncomment to see both)
        // glBindVertexArray(rectangleVAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Rotate over time
        // glm::mat4 trans = getTransformMatrix((float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 0.0f),
        // glm::vec3(0.5f, 0.5f, 0.5f));
        // GLint transformLoc = glGetUniformLocation(ourShader.getShaderProgramID(), "transform");
        // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup all resources
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteVertexArrays(1, &rectangleVAO);
    glDeleteBuffers(1, &triangleVBO);
    glDeleteBuffers(1, &rectangleVBO);
    glDeleteBuffers(1, &rectangleEBO);
    glfwTerminate();
    return 0;
}
