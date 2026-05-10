// AnimatedEntity.cpp
#include "StdAfx.h"
#include "AnimatedEntity.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Maths.h"

AnimatedEntity::AnimatedEntity(Model* model, Animation* startAnimation)
    : m_Model(model), m_Animator(new Animator(startAnimation)),
    m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f) {
}

AnimatedEntity::~AnimatedEntity() {
    delete m_Animator;
}

void AnimatedEntity::Update(float deltaTime) {
    if (m_Animator) {
        m_Animator->UpdateAnimation(deltaTime);
    }
}

void AnimatedEntity::PlayAnimation(Animation* animation) {
    if (m_Animator) {
        m_Animator->PlayAnimation(animation);
    }
}

glm::mat4 AnimatedEntity::GetModelMatrix() const {
    glm::mat4 transformationMartix = Maths::CreateTransformationMatrix(m_Position, m_Rotation, m_Scale);
    return transformationMartix;
}