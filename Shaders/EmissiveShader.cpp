#include "StdAfx.h"
#include "EmissiveShader.h"
#include "Camera.h"
#include "Maths.h"

const char* emissiveVertexFile = "Shaders\\emissive.vs";
const char* emissiveFragmentFile = "Shaders\\emissive.fs";

EmissiveShader::EmissiveShader()
{
    LoadShader(emissiveVertexFile, emissiveFragmentFile);
}

void EmissiveShader::BindAttributes()
{
    BindAttribute(0, "position");
    BindAttribute(1, "textureCoords");
}

void EmissiveShader::GetAllUniformLocations()
{
    m_location_transformationMatrix = GetUniformLocation("transformationMatrix");
    m_location_viewMatrix = GetUniformLocation("viewMatrix");
    m_location_projectionMatrix = GetUniformLocation("projectionMatrix");
    m_location_textureSampler = GetUniformLocation("textureSampler");
    m_location_emissiveColor = GetUniformLocation("emissiveColor");
    m_location_numberOfRows = GetUniformLocation("numberOfRows");
    m_location_offset = GetUniformLocation("offset");
}

void EmissiveShader::LoadTransformationMatrix(const glm::mat4& matrix)
{
    LoadMatrix(m_location_transformationMatrix, matrix);
}

void EmissiveShader::LoadViewMatrix(Camera* camera)
{
    glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
    LoadMatrix(m_location_viewMatrix, viewMatrix);
}

void EmissiveShader::LoadProjectionMatrix(const glm::mat4& projMatrix)
{
    LoadMatrix(m_location_projectionMatrix, projMatrix);
}

void EmissiveShader::LoadTexture()
{
    LoadInt(m_location_textureSampler, 0);   // ÎÆÀíµ¥Ôª 0
}

void EmissiveShader::LoadEmissiveColor(const glm::vec3& color)
{
    LoadVector(m_location_emissiveColor, color);
}

void EmissiveShader::LoadNumberOfRows(int rows)
{
    LoadFloat(m_location_numberOfRows, (float)rows);
}

void EmissiveShader::LoadOffset(float x, float y)
{
    LoadVector(m_location_offset, glm::vec2(x, y));
}