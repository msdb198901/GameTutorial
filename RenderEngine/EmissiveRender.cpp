#include "StdAfx.h"
#include "EmissiveRender.h"
#include "EmissiveShader.h"
#include "EmissiveEntity.h"
#include "RawModel.h"
#include "TextureModel.h"
#include "Texture.h"
#include "Maths.h"

EmissiveRender::EmissiveRender(EmissiveShader* shader, const glm::mat4& projectionMatrix)
    : m_shader(shader)
{
    m_shader->Start();
    m_shader->LoadProjectionMatrix(projectionMatrix);
    m_shader->LoadTexture();      // 固定纹理单元0
    m_shader->Stop();
}

EmissiveRender::~EmissiveRender()
{
}

void EmissiveRender::RenderModel(std::list<EmissiveEntity*>& entities)
{
    for (auto entity : entities)
    {
        PrepareEntity(entity);
        LoadModelMatrix(entity);

        RawModel* model = entity->GetTextureModel()->GetRawModel();
        glDrawElements(GL_TRIANGLES, model->GetVertexCount(), GL_UNSIGNED_INT, 0);

        UnbindEntity();
    }
}

void EmissiveRender::PrepareEntity(EmissiveEntity* entity)
{
    RawModel* model = entity->GetTextureModel()->GetRawModel();
    glBindVertexArray(model->GetVAOID());
    glEnableVertexAttribArray(0);  // position
    glEnableVertexAttribArray(1);  // textureCoords

    // 激活纹理单元0并绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, entity->GetTextureModel()->GetTexture()->GetID());

    // 加载自发光颜色
    m_shader->LoadEmissiveColor(entity->GetEmissiveColor());

    // 处理纹理图集（如果纹理有多个子图）
    Texture* tex = entity->GetTextureModel()->GetTexture();
    int numberOfRows = tex->GetNumberOfRows();
    m_shader->LoadNumberOfRows(numberOfRows);

    float xOffset = entity->GetTextureXOffset();
    float yOffset = entity->GetTextureYOffset();
    m_shader->LoadOffset(xOffset, yOffset);
}

void EmissiveRender::LoadModelMatrix(EmissiveEntity* entity)
{
    glm::mat4 transformation = Maths::CreateTransformationMatrix(
        entity->GetPosition(),
        entity->GetRotation(),
        entity->GetScale()
    );
    m_shader->LoadTransformationMatrix(transformation);
}

void EmissiveRender::UnbindEntity()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}

void EmissiveRender::CleanUp()
{
    // 无需额外清理
}