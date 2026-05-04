#pragma once

#include "Entity.h"

class EmissiveEntity : public Entity
{
public:
    // 构造函数：复用 Entity 的构造，额外增加自发光颜色
    EmissiveEntity(TextureModel* model, glm::vec3 position, glm::vec3 rotation, float scale,
        const glm::vec3& emissiveColor = glm::vec3(1.0f, 0.85f, 0.55f));

    EmissiveEntity(TextureModel* model, int textureId, glm::vec3 position, glm::vec3 rotation, float scale,
        const glm::vec3& emissiveColor = glm::vec3(1.0f, 0.85f, 0.55f));

    // 自发光颜色操作
    void SetEmissiveColor(const glm::vec3& color);
    glm::vec3 GetEmissiveColor() const;

private:
    glm::vec3 m_emissiveColor;
};