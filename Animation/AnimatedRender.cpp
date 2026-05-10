// AnimatedRenderer.cpp
#include "StdAfx.h"
#include "AnimatedRender.h"

const char* vertexPath = "Animation\\animated.vs";
const char* fragmentPath = "Animation\\animated.fs";

AnimatedRender::AnimatedRender(const glm::mat4& projectionMatrix)
    : m_Shader(vertexPath, fragmentPath), m_Projection(projectionMatrix) {
    m_Shader.use();
    m_Shader.setMat4("projection", m_Projection);
}

void AnimatedRender::Render(const std::vector<AnimatedEntity*>& entities,
    const glm::mat4& viewMatrix,
    const glm::vec3& cameraPosition) {
    m_Shader.use();
    m_Shader.setMat4("view", viewMatrix);
    // 如果着色器需要光源位置等，可以在这里设置
    // m_Shader.setVec3("viewPos", cameraPosition);

    for (auto* entity : entities) {
        // 更新骨骼矩阵到着色器
        auto transforms = entity->GetAnimator()->GetFinalBoneMatrices();
        for (size_t i = 0; i < transforms.size(); ++i) {
            m_Shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        }

        // 设置模型矩阵
        glm::mat4 modelMatrix = entity->GetModelMatrix();
        m_Shader.setMat4("model", modelMatrix);

        // 绘制模型
        entity->GetModel()->Draw(m_Shader);
    }
}