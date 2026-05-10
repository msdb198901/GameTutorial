// AnimatedShader.h
#pragma once
#include "ShaderProgram.h"

class AnimatedShader : public ShaderProgram {
public:
    AnimatedShader();
    ~AnimatedShader();

    void BindAttributes() override;
    void GetAllUniformLocations() override;

    void LoadProjectionMatrix(const glm::mat4& projection);
    void LoadViewMatrix(const glm::mat4& view);
    void LoadModelMatrix(const glm::mat4& model);
    void LoadBoneMatrices(const std::vector<glm::mat4>& boneMatrices);
    void LoadTextureUnit(int unit);  // 纹理单元，通常为 0

private:
    int m_location_projectionMatrix;
    int m_location_viewMatrix;
    int m_location_modelMatrix;
    int m_location_textureSampler;
    // 骨骼矩阵数组（最多 100 个）
    int m_location_boneMatrices[100];
};