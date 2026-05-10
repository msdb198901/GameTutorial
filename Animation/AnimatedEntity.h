// AnimatedEntity.h
#pragma once
#include <glm/glm.hpp>
#include <learnopengl/model.h>
#include <learnopengl/animator.h>

class AnimatedEntity {
public:
    AnimatedEntity(Model* model, Animation* startAnimation);
    ~AnimatedEntity();

    void Update(float deltaTime);
    void PlayAnimation(Animation* animation);

    void SetPosition(const glm::vec3& pos) { m_Position = pos; }
    void SetRotation(const glm::vec3& rot) { m_Rotation = rot; }
    void SetScale(float scale) { m_Scale = scale; }
    glm::mat4 GetModelMatrix() const;

    Model* GetModel() const { return m_Model; }
    Animator* GetAnimator() const { return m_Animator; }

    glm::vec3 GetPosition() const { return m_Position; }

private:
    Model* m_Model;           // 注意：Model 对象需外部管理生命周期（如 ResourceManager）
    Animator* m_Animator;
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    float m_Scale;
};