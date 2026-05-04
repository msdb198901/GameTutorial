#pragma once

#include "ShaderProgram.h"

class Camera;
class EmissiveShader : public ShaderProgram
{
public:
    EmissiveShader();

    virtual void BindAttributes() override;
    virtual void GetAllUniformLocations() override;

    void LoadTransformationMatrix(const glm::mat4& matrix);
    void LoadViewMatrix(Camera* camera);   // 直接从 camera 获取更方便
    void LoadProjectionMatrix(const glm::mat4& projMatrix);
    void LoadTexture();                                 // 固定纹理单元 0
    void LoadEmissiveColor(const glm::vec3& color);     // 可选，让每个灯不同颜色

    // 如果想支持 atlas（如 MultipleTextureModel），可以保留 numberOfRows 和 offset
    void LoadNumberOfRows(int rows);
    void LoadOffset(float x, float y);

private:
    int m_location_transformationMatrix;
    int m_location_viewMatrix;
    int m_location_projectionMatrix;
    int m_location_textureSampler;
    int m_location_emissiveColor;
    int m_location_numberOfRows;
    int m_location_offset;
};