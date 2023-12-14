#include "platform/OpenGL/GLShader.h"
#include "GLShader.h"

#include <fstream>
#include <sstream>
#include <iostream>

GLShader::GLShader(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        // spdlog::critical("Error while reading file: {}", e.what());
        throw std::runtime_error("unable to compile shader!");
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    // shader Program
    m_descriptor = glCreateProgram();
    glAttachShader(m_descriptor, vertex);
    glAttachShader(m_descriptor, fragment);
    glLinkProgram(m_descriptor);
    CheckCompileErrors(m_descriptor, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

GLShader::~GLShader()
{
    glDeleteProgram(m_descriptor);
}

void GLShader::Bind()
{
    glUseProgram(m_descriptor);
}

void GLShader::Unbind()
{
    glUseProgram(0);
}

void GLShader::SetInt(const std::string &name, const int value)
{
    glUniform1i(GetLocation(name), value);
}

void GLShader::SetBool(const std::string &name, const bool value)
{
    SetInt(name, value);
}

void GLShader::SetFloat(const std::string &name, const float value)
{
    glUniform1f(GetLocation(name), value);
}

void GLShader::SetFloat2(const std::string &name, const glm::vec2 &value)
{
    glUniform2f(GetLocation(name), value.x, value.y);
}

void GLShader::SetFloat3(const std::string &name, const glm::vec3 &value)
{
    glUniform3f(GetLocation(name), value.x, value.y, value.z);
}

void GLShader::SetFloat4(const std::string &name, const glm::vec4 &value)
{
    glUniform4f(GetLocation(name), value.x, value.y, value.z, value.w);
}

void GLShader::SetMat3(const std::string &name, const glm::mat3 &value)
{
    glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, &value[0][0]);
}

void GLShader::SetMat4(const std::string &name, const glm::mat4 &value)
{
    glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &value[0][0]);
}

void GLShader::BindUniformBlock(const std::string &name, const int32_t index)
{
        unsigned int map_data = glGetUniformBlockIndex(m_descriptor, name.c_str());
    glUniformBlockBinding(m_descriptor, map_data, 0);
    // glUniformBlockBinding(m_descriptor, glGetUniformBlockIndex(m_descriptor, name.c_str()), index);
}

GLint GLShader::GetLocation(const std::string &name)
{
    return glGetUniformLocation(m_descriptor, name.c_str());
}


void GLShader::CheckCompileErrors(GLuint shader, const std::string &type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            // spdlog::critical("{} shader compilation error!", type);
            // spdlog::critical("\n{}", infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            // spdlog::critical("{} shader compilation error!", type);
            // spdlog::critical("\n{}", infoLog);
        }
    }
}