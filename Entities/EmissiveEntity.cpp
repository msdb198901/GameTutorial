#include "StdAfx.h"
#include "EmissiveEntity.h"

EmissiveEntity::EmissiveEntity(TextureModel* model, glm::vec3 position, glm::vec3 rotation, float scale,
    const glm::vec3& emissiveColor)
    : Entity(model, position, rotation, scale), m_emissiveColor(emissiveColor)
{
}

EmissiveEntity::EmissiveEntity(TextureModel* model, int textureId, glm::vec3 position, glm::vec3 rotation, float scale,
    const glm::vec3& emissiveColor)
    : Entity(model, textureId, position, rotation, scale), m_emissiveColor(emissiveColor)
{
}

void EmissiveEntity::SetEmissiveColor(const glm::vec3& color)
{
    m_emissiveColor = color;
}

glm::vec3 EmissiveEntity::GetEmissiveColor() const
{
    return m_emissiveColor;
}