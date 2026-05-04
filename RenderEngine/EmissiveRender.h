#pragma once

#include <list>

class EmissiveShader;
class EmissiveEntity;

class EmissiveRender
{
public:
    EmissiveRender(EmissiveShader* shader, const glm::mat4& projectionMatrix);
    ~EmissiveRender();

    void RenderModel(std::list<EmissiveEntity*>& entities);

    void CleanUp();

private:
    void PrepareEntity(EmissiveEntity* entity);
    void LoadModelMatrix(EmissiveEntity* entity);
    void UnbindEntity();

    EmissiveShader* m_shader;
};