#include "Shader.h"
#include "../util.hpp"
#include <fstream>
#include <sstream>

// Thank our lord learnopengl.com
int checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    return success;
}

void Shader::Load(std::string vertexPath, std::string fragmentPath) {
    /*std::ifstream vertexSource, fragmentSource;
    std::string vertexCode, fragmentCode;
    std::stringstream vertexStream, fragmentStream;

    // learnopengl.com wants this so we do it
    vertexSource.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentSource.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexSource.open(vertexPath);
        fragmentSource.open(fragmentPath);
        // read file
        vertexStream   << vertexSource.rdbuf();
        fragmentStream << fragmentSource.rdbuf();
        // close file
        vertexSource.close();
        fragmentSource.close();
        // convert stream char* to strng
        vertexCode   = vertexStream.str();
        fragmentCode = fragmentStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER:FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }*/
    std::string vertexSource = ReadTextFromFile("resources/shaders/" + vertexPath);
    std::string fragmentSource = ReadTextFromFile("resources/shaders/" + fragmentPath);

    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();

    unsigned int vertex, fragment;

    // Create the two shaders and then link them
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    int shaderID = glCreateProgram();
    glAttachShader(shaderID, vertex);
    glAttachShader(shaderID, fragment);
    glLinkProgram(shaderID);

    // If there is already a shader, then delete it and make the one we just made the shader.
    if (checkCompileErrors(shaderID, "PROGRAM")) {
        if (ID != -1) {
            glDeleteProgram(ID);
        }
        ID = shaderID;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetMat4(const std::string& name, glm::mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}