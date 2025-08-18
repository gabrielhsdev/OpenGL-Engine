#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // Required for OpenGL function pointers
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class Shader
 * @brief Utility class for compiling and managing OpenGL shader programs.
 *
 * This class handles reading shader source files, compiling vertex/fragment shaders,
 * linking them into a program, and setting uniform variables.
 */
class Shader {
  public:
    /**
     * @struct ShaderType
     * @brief Represents a compiled shader object.
     */
    struct ShaderType {
        unsigned int id; ///< OpenGL shader object ID
        GLenum type;     ///< Type of shader (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
    };

    /**
     * @brief Construct a Shader program from vertex and fragment shader source files.
     * @param vertexPath Path to the vertex shader source file.
     * @param fragmentPath Path to the fragment shader source file.
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Destructor. Deletes the shader program.
     */
    ~Shader();

    /**
     * @brief Activate the shader program.
     */
    void use() const;

    /**
     * @brief Set a boolean uniform.
     * @param name Uniform name in the shader.
     * @param value Boolean value to set.
     */
    void setBool(const std::string& name, bool value) const;

    /**
     * @brief Set an integer uniform.
     * @param name Uniform name in the shader.
     * @param value Integer value to set.
     */
    void setInt(const std::string& name, int value) const;

    /**
     * @brief Set a float uniform.
     * @param name Uniform name in the shader.
     * @param value Float value to set.
     */
    void setFloat(const std::string& name, float value) const;

    /**
     * @brief Set a 4x4 matrix uniform.
     * @param name Uniform name in the shader.
     * @param mat 4x4 matrix to set.
     */
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    /**
     * @brief Get the OpenGL shader program ID.
     * @param shaderProgramID Reference to store the shader program ID.
     */
    unsigned int getShaderProgramID();

  private:
    unsigned int SHADERPROGRAMID; ///< OpenGL shader program ID

    static const int INFOLOG_SIZE = 512; ///< Max size of shader compiler log

    /**
     * @brief Read shader source code from a file.
     * @param shaderPath Path to shader source file.
     * @return String stream containing the shader source.
     */
    static std::stringstream readShaderFile(const char* shaderPath);

    /**
     * @brief Compile a shader from source.
     * @param shaderCode Shader source code.
     * @param shaderType Shader type (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
     * @return Compiled shader object.
     */
    static ShaderType compileShader(const char* shaderCode, GLenum shaderType);

    /**
     * @brief Link a vertex and fragment shader into a program.
     * @param vertex Compiled vertex shader.
     * @param fragment Compiled fragment shader.
     * @return OpenGL shader program ID.
     */
    static unsigned int createShaderProgram(const ShaderType& vertex, const ShaderType& fragment);
};

#endif
