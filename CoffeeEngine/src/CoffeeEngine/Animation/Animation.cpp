#include "Animation.h"

namespace Coffee {

    Animation::Animation(std::string name)
        : Resource(ResourceType::Animation), m_Name(std::move(name)) {}

    void Animation::SetAnimation(std::unique_ptr<ozz::animation::Animation> animation) {
        m_Animation = std::move(animation);
        if (m_Animation) {
            m_Name = m_Animation->name();
        }
    }

    AnimationController::AnimationController(std::vector<Animation>&& animations,
                                             std::map<std::string, unsigned int>&& animationsMap)
        : Resource(ResourceType::AnimationController),
          m_Animations(std::move(animations)), 
          m_AnimationsMap(std::move(animationsMap))
    {
    }

    void AnimationController::AddAnimation(const std::string& name,
                                           std::unique_ptr<ozz::animation::Animation> animation) {
        m_AnimationsMap[name] = static_cast<unsigned int>(m_Animations.size());
        Animation newAnimation(name);
        newAnimation.SetAnimation(std::move(animation));
        m_Animations.push_back(std::move(newAnimation));
    }

    Animation* AnimationController::GetAnimation(const std::string& name) {
        auto it = m_AnimationsMap.find(name);
        if (it != m_AnimationsMap.end()) {
            return &m_Animations[it->second];
        }
        return nullptr;
    }

    Animation* AnimationController::GetAnimation(unsigned int index) {
        if (index < m_Animations.size()) {
            return &m_Animations[index];
        }
        return nullptr;
    }

}  // namespace Coffee
