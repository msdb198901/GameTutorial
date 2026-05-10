
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <learnopengl/shader.h>
#include "AnimatedEntity.h"

class AnimatedRender {
public:
    AnimatedRender(const glm::mat4& projectionMatrix);
    void Render(const std::vector<AnimatedEntity*>& entities,
        const glm::mat4& viewMatrix,
        const glm::vec3& cameraPosition); // cameraPosition 可选，用于光照

private:
    Shader m_Shader;      // LearnOpenGL 风格的 shader
    glm::mat4 m_Projection;
};