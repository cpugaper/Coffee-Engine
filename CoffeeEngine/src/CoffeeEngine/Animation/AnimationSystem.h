#pragma once

#include "Animation.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "Skeleton.h"
#include "glad/glad.h"

#include <glm/gtc/type_ptr.hpp>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/blending_job.h>

namespace Coffee {

    class AnimationSystem : public Resource
    {
    public:
        AnimationSystem();
        ~AnimationSystem() = default;

        void Update(float deltaTime);
        void SetCurrentAnimation(const std::string& name);
        void SetCurrentAnimation(unsigned int index);

        float GetCurrentTime() const { return m_AnimationTime; }
        unsigned int GetCurrentAnimationIndex() const { return m_CurrentAnimation; }

        void SetSkeleton(Ref<Skeleton> skeleton) { m_Skeleton = skeleton; }
        Ref<Skeleton> GetSkeleton() const { return m_Skeleton; }

        void SetAnimationController(Ref<AnimationController> controller) { m_AnimationController = controller; }
        Ref<AnimationController> GetAnimationController() const { return m_AnimationController; }
        void SetBoneTransformations(const Ref<Shader>& shader);


        //Getters y setters
        float GetBlendDuration() const { return m_BlendDuration; }
        void SetBlendDuration(float value) { m_BlendDuration = value; }

        float GetBlendThreshold() const { return m_BlendThreshold; }
        void SetBlendThreshold(float value) { m_BlendThreshold = value; }

        float GetAnimationSpeed() const { return m_AnimationSpeed; }
        void SetAnimationSpeed(float speed) { m_AnimationSpeed = speed; }

        
        bool IsPlaying() const { return m_isPlaying; }
        void SetPlaying(bool isPlaying) { m_isPlaying = isPlaying; }

        
        bool IsLooping() const { return m_IsLooping; }
        void SetLooping(bool isLooping) { m_IsLooping = isLooping; }

        
        bool IsFinished() const { return m_IsFinished; }
        void SetFinished(bool isFinished) { m_IsFinished = isFinished; }

    private:
        void SampleAnimation(float deltaTime, const ozz::animation::Animation& animation, const ozz::animation::Skeleton& skeleton, const std::vector<Joint>& joints, std::vector<glm::mat4>& jointMatrices);
        void BlendAnimations(float deltaTime, const ozz::animation::Skeleton& skeleton, const std::vector<Joint>& joints, std::vector<glm::mat4>& jointMatrices);

        void PrintFloat4x4(const ozz::math::Float4x4& matrix);

        static glm::mat4 OzzToGlmMat4(const ozz::math::Float4x4& from) {
            glm::mat4 to;
            memcpy(glm::value_ptr(to), &from.cols[0], sizeof(glm::mat4));
            return to;
        }

    private:
        friend class cereal::access;

        template <class Archive> void save(Archive& archive) const
        {
            
            //UUID materialUUID = m_Material->GetUUID();
            //Skeleton, AnimationController
            //A lo mejor no, context, layer, blendingJob


            //archive(m_Vertices, m_Indices, m_AABB, materialUUID, cereal::base_class<Resource>(this));
            archive(m_CurrentAnimation, m_NextAnimation, m_AnimationTime, m_NextAnimationTime, m_BlendTime, m_BlendDuration, m_BlendThreshold, m_IsBlending, m_AnimationSpeed, m_isPlaying, m_IsLooping, m_IsFinished, m_Skeleton, m_AnimationController, cereal::base_class<Resource>(this));
        }

        template <class Archive> void load(Archive& archive)
        {
            //UUID materialUUID;
            // 
            // archive(m_Vertices, m_Indices, m_AABB, materialUUID, cereal::base_class<Resource>(this));
            // 
            //m_Material = ResourceLoader::LoadMaterial(materialUUID);
            //Cargar skeleton, animation controller
            //A lo mejor no, context, layer, blendingJob

             archive(m_CurrentAnimation, m_NextAnimation, m_AnimationTime, m_NextAnimationTime, m_BlendTime,
                    m_BlendDuration, m_BlendThreshold, m_IsBlending, m_AnimationSpeed, m_isPlaying, m_IsLooping,
                    m_IsFinished, m_Skeleton, m_AnimationController, cereal::base_class<Resource>(this));

            
        }

        template <class Archive> static void load_and_construct(Archive& data, cereal::construct<AnimationSystem>& construct)
        {
            
            construct();

            
            data(construct->m_CurrentAnimation, construct->m_NextAnimation, construct->m_AnimationTime,
                 construct->m_NextAnimationTime, construct->m_BlendTime, construct->m_BlendDuration,
                 construct->m_BlendThreshold, construct->m_IsBlending, construct->m_AnimationSpeed,
                 construct->m_isPlaying, construct->m_IsLooping, construct->m_IsFinished, construct->m_Skeleton,
                 construct->m_AnimationController,
                 cereal::base_class<Resource>(construct.ptr()));
        }


    private:
        GLuint boneMatricesUBO;
        Ref<Skeleton> m_Skeleton;
        Ref<AnimationController> m_AnimationController;
        ozz::animation::SamplingJob::Context m_Context;
        ozz::animation::BlendingJob::Layer m_BlendLayers[2];
        ozz::animation::BlendingJob m_BlendJob;
        unsigned int m_CurrentAnimation = 0;
        unsigned int m_NextAnimation = 0;
        float m_AnimationTime = 0.f;
        float m_NextAnimationTime = 0.f;
        float m_BlendTime = 0.f;
        float m_BlendDuration = 0.25f;
        float m_BlendThreshold = 0.8;
        bool m_IsBlending = false;

        float m_AnimationSpeed = 1.0f;
        bool m_isPlaying = true;
        bool m_IsLooping = true;
        bool m_IsFinished = false;


    };
} // namespace Coffee
CEREAL_REGISTER_TYPE(Coffee::AnimationSystem);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Resource, Coffee::AnimationSystem);