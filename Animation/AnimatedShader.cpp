// AnimatedShader.cpp
#include "AnimatedShader.h"

AnimatedShader::AnimatedShader() {
    // 你需要提供自己的顶点/片段着色器文件路径
    LoadShader("Shaders/animated.vs", "Shaders/animated.fs");
}

AnimatedShader::~AnimatedShader() {}

void AnimatedShader::BindAttributes() {
    BindAttribute(0, "position");
    BindAttribute(1, "texCoords");
    BindAttribute(2, "normal");
    BindAttribute(3, "boneIDs");
    BindAttribute(4, "boneWeights");
}

void AnimatedShader::GetAllUniformLocations() {
    m_location_projectionMatrix = GetUniformLocation("projectionMatrix");
    m_location_viewMatrix = GetUniformLocation("viewMatrix");
    m_location_modelMatrix = GetUniformLocation("modelMatrix");
    m_location_textureSampler = GetUniformLocation("textureSampler");

    for (int i = 0; i < 100; ++i) {
        std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
        m_location_boneMatrices[i] = GetUniformLocation(name.c_str());
        // 如果某个位置不存在（超出实际骨骼数），后面 LoadBoneMatrices 中会跳过 -1
    }
}

void AnimatedShader::LoadProjectionMatrix(const glm::mat4& projection) {
    LoadMatrix(m_location_projectionMatrix, projection);
}

void AnimatedShader::LoadViewMatrix(const glm::mat4& view) {
    LoadMatrix(m_location_viewMatrix, view);
}

void AnimatedShader::LoadModelMatrix(const glm::mat4& model) {
    LoadMatrix(m_location_modelMatrix, model);
}

void AnimatedShader::LoadBoneMatrices(const std::vector<glm::mat4>& boneMatrices) {
    for (size_t i = 0; i < boneMatrices.size(); ++i) {
        if (m_location_boneMatrices[i] != -1) {
            LoadMatrix(m_location_boneMatrices[i], boneMatrices[i]);
        }
    }
}

void AnimatedShader::LoadTextureUnit(int unit) {
    LoadInt(m_location_textureSampler, unit);
}