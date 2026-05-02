#pragma once

class TextureModel;
class Entity
{
public:
    Entity(TextureModel *model, glm::vec3 position, glm::vec3 rotation, float scale);

    TextureModel* GetTextureModel() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    float GetScale() const;

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 rotation);
    void SetScale(float scale);

    void IncreasePosition(float x, float y, float z);
    void IncreaseRotation(float x, float y, float z);

private:
    TextureModel*  model;
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;
};
