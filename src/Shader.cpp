#include "shaders/Shader.h"

std::stringstream Shader::readShaderFile(const char* shaderPath) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream;
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << '\n';
        return std::stringstream();
    }
}

Shader::ShaderType Shader::compileShader(const char* shaderCode, GLenum shaderType) {
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[Shader::INFOLOG_SIZE];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(shader, Shader::INFOLOG_SIZE, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << '\n';
        return ShaderType{0, shaderType}; // Return an invalid ShaderType on failure
    }

    return ShaderType{shader, shaderType};
}

unsigned int Shader::createShaderProgram(const ShaderType& vertex, const ShaderType& fragment) {
    unsigned int programId = glCreateProgram();
    char infoLog[Shader::INFOLOG_SIZE];
    int success;

    glAttachShader(programId, vertex.id);
    glAttachShader(programId, fragment.id);
    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (success == 0) {
        glGetProgramInfoLog(programId, Shader::INFOLOG_SIZE, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
        return 0; // Return 0 on failure
    }

    glDeleteShader(vertex.id);
    glDeleteShader(fragment.id);

    std::cout << "Shader program created successfully with programId: " << programId << '\n';
    return programId;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    unsigned int programId;
    std::string vertexCode;
    std::string fragmentCode;
    std::stringstream vShaderStream = readShaderFile(vertexPath);
    std::stringstream fShaderStream = readShaderFile(fragmentPath);

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Next we need to compile the shaders
    ShaderType vertex;
    ShaderType fragment;

    vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
    if (vertex.id == 0) {
        std::cerr << "Failed to compile vertex shader.\n";
        return; // Early exit if shader compilation fails
    }

    fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);
    if (fragment.id == 0) {
        std::cerr << "Failed to compile fragment shader.\n";
        return; // Early exit if shader compilation fails
    }

    programId = createShaderProgram(vertex, fragment);
    if (programId == 0) {
        std::cerr << "Failed to create shader program.\n";
        return; // Early exit if shader program creation fails
    }

    this->SHADERPROGRAMID = programId;

    std::cout << "Shader program created successfully with SHADERPROGRAMID: "
              << this->SHADERPROGRAMID << '\n';
}

Shader::~Shader() {
    if (SHADERPROGRAMID != 0) {
        glDeleteProgram(SHADERPROGRAMID);
    }
}

void Shader::use() const {
    if (SHADERPROGRAMID == 0) {
        std::cerr
            << "WARNING: Attempting to use a shader program with SHADERPROGRAMID 0. Make sure "
               "the shader "
               "was compiled and linked successfully.\n";
    }
    glUseProgram(this->SHADERPROGRAMID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(this->SHADERPROGRAMID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(this->SHADERPROGRAMID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(this->SHADERPROGRAMID, name.c_str()), value);
}
