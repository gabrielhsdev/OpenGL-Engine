
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// TODO: Create a function to initialize only the VAO
// TODO: Create a function to initialize only the VBO to be used for rendering a triangle without EBO

/**
 * @brief Callback function to adjust the viewport when the window size changes.
 * @param window A pointer to the GLFWwindow that received the event.
 * @param width The new width, in pixels, of the window.
 * @param height The new height, in pixels, of the window.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * @brief Processes user input, specifically checking for the ESCAPE key to close the window.
 * @param window A pointer to the GLFWwindow to check for input.
 */
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS forward compat
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE); // macOS retina

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Compiles a shader from its source code.
 * @param type The type of shader to compile (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
 * @param source The source code string of the shader.
 * @return The ID of the compiled shader, or 0 if compilation fails.
 */
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

/**
 * @brief Links vertex and fragment shaders into a shader program.
 * @param vertexSrc The source code for the vertex shader.
 * @param fragmentSrc The source code for the fragment shader.
 * @return The ID of the linked shader program, or 0 if linking fails.
 */
unsigned int createShaderProgram(const char* vertexSrc, const char* fragmentSrc) {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

/**
 * @brief Creates a Vertex Array Object (VAO) and Vertex Buffer Object (VBO) for a single triangle.
 * @param VAO Reference to an unsigned int to store the generated VAO ID.
 * @param VBO Reference to an unsigned int to store the generated VBO ID.
 */
void createTriangleVAO(unsigned int &VAO, unsigned int &VBO) {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left 
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top   
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

/**
 * @brief Creates a Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO) for a rectangle (two triangles).
 * @param VAO Reference to an unsigned int to store the generated VAO ID.
 * @param VBO Reference to an unsigned int to store the generated VBO ID.
 * @param EBO Reference to an unsigned int to store the generated EBO ID.
 */
void createTriangleEBO(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO) {
    // Element Buffer Objects (EBOs) allow us to reuse vertices for multiple triangles.
    // Thankfully, element buffer objects work exactly like that. 
    // An EBO is a buffer, just like a vertex buffer object, that stores indices that 
    //OpenGL uses to decide what vertices to draw
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    // Create VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Do not unbind the EBO here! It stays bound to the VAO
    glBindVertexArray(0);
}

/**
 * @brief Renders a triangle using glDrawArrays.
 * @param shaderProgram The ID of the shader program to use for rendering.
 * @param VAO The ID of the Vertex Array Object containing the triangle's data.
 */
void renderTriangle(unsigned int shaderProgram, unsigned int VAO) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

/**
 * @brief Renders a shape using glDrawElements (with an EBO).
 * @param shaderProgram The ID of the shader program to use for rendering.
 * @param VAO The ID of the Vertex Array Object containing the shape's data and EBO.
 */
void renderTriangleEBO(unsigned int shaderProgram, unsigned int VAO) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

/**
 * @brief Cleans up OpenGL resources (VAO, VBO, EBO, shader program).
 * @param VAO The ID of the Vertex Array Object to delete.
 * @param VBO The ID of the Vertex Buffer Object to delete.
 * @param EBO The ID of the Element Buffer Object to delete.
 * @param shaderProgram The ID of the shader program to delete.
 */
void cleanup(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int shaderProgram) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

/**
 * @brief Main function of the OpenGL application.
 * Initializes GLFW, GLAD, sets up the window, compiles and links shaders, prepares vertex data,
 * enters the main rendering loop, and cleans up resources upon exit.
 * @return 0 if the program executes successfully, -1 otherwise.
 */
int main() {
    GLFWwindow* window = initWindow(1280, 720, "LearnOpenGL");
    if (!window) return -1;

    if (!initGlad()) return -1;

    // Get actual framebuffer size (important for high-DPI)
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Shader sources
    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    unsigned int VAO, VBO, EBO;
    // createTriangleVAO(VAO, VBO); // This should be used if not using EBO
    createTriangleEBO(VAO, VBO, EBO); // Use this if using EBO

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        // renderTriangle(shaderProgram, VAO); // This should be used if not using EBO
        renderTriangleEBO(shaderProgram, VAO); // Use this if using EBO
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanup(VAO, VBO, EBO, shaderProgram);
    glfwTerminate();
    return 0;
}


