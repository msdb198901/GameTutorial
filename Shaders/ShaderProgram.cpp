#include "StdAfx.h"
#include "ShaderProgram.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void checkCompileErrors(unsigned int shader, std::string type)
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
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void ShaderProgram::LoadShader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
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
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders

    // vertex shader
    m_vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vertexID, 1, &vShaderCode, NULL);
    glCompileShader(m_vertexID);
    checkCompileErrors(m_vertexID, "VERTEX");
    // fragment Shader
    m_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_fragmentID, 1, &fShaderCode, NULL);
    glCompileShader(m_fragmentID);
    checkCompileErrors(m_fragmentID, "FRAGMENT");
    // shader Program
    m_programID = glCreateProgram();
    glAttachShader(m_programID, m_vertexID);
    glAttachShader(m_programID, m_fragmentID);

    BindAttributes();

    glLinkProgram(m_programID);
    checkCompileErrors(m_programID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
        
    GetAllUniformLocations();
}

void ShaderProgram::Start()
{
    glUseProgram(m_programID);
}

void ShaderProgram::Stop()
{
    glUseProgram(0);
}

void ShaderProgram::CleanUp()
{
    Stop();
    glDetachShader(m_programID, m_vertexID);
    glDetachShader(m_programID, m_fragmentID);
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
    glDeleteProgram(m_programID);
}

void ShaderProgram::BindAttribute(int attribute, const char* variable)
{
    glBindAttribLocation(m_programID, attribute, variable);
}

int ShaderProgram::GetUniformLocation(const char* uniformName)
{
    return glGetUniformLocation(m_programID, uniformName);
}

void ShaderProgram::LoadFloat(int location, float value)
{
    glUniform1f(location, value);
}
void ShaderProgram::LoadVector(int location, const glm::vec2& vector)
{
    glUniform2f(location, vector.x, vector.y);
}
void ShaderProgram::LoadVector(int location, const glm::vec3& vector)
{
    glUniform3f(location, vector.x, vector.y, vector.z);
}
void ShaderProgram::LoadVector(int location, const glm::vec4& vector)
{
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}
void ShaderProgram::LoadMatrix(int location, const glm::mat4& matrix)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
void ShaderProgram::LoadBool(int location, bool value)
{
    glUniform1i(location, (int)value);
}