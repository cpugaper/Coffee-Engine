#pragma once

#include <ozz/animation/runtime/animation.h>
#include <ozz/base/memory/unique_ptr.h>

#include <map>
#include <string>
#include <vector>

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp> 

namespace Coffee
{
    class Animation : public Resource
    {
      public:
        Animation() = default;
        //Animation(std::string name, std::unique_ptr<ozz::animation::Animation> animation);
        Animation(std::string name);
        ~Animation() = default;

        Animation(const Animation&) = delete;
        Animation& operator=(const Animation&) = delete;

        Animation(Animation&& other) noexcept
            : m_Name(std::move(other.m_Name)), m_Animation(std::move(other.m_Animation))
        {
        }

        Animation& operator=(Animation&& other) noexcept
        {
            m_Name = std::move(other.m_Name);
            m_Animation = std::move(other.m_Animation);
            return *this;
        }

        const std::string& GetName() const { return m_Name; }
        float GetDuration() const { return m_Animation->duration(); }

        void SetAnimation(std::unique_ptr<ozz::animation::Animation> animation);

        const ozz::animation::Animation* GetAnimation() const { return m_Animation.get(); }

      //private:
      //  friend class cereal::access;

      //  template <class Archive> void save(Archive& archive) const
      //  {
      //      

      //      //archive(m_Name, m_Animation, cereal::base_class<Resource>(this));
      //      archive(m_Name, cereal::base_class<Resource>(this));
      //      
      //  }

      //  template <class Archive> void load(Archive& archive)
      //  {
      //      //archive(m_Name, m_Animation, cereal::base_class<Resource>(this));
      //      archive(m_Name, cereal::base_class<Resource>(this));

      //  }

      //  template <class Archive>
      //  static void load_and_construct(Archive& archive, cereal::construct<Animation>& construct)
      //  {
      //      //std::string name;
      //      //std::unique_ptr<ozz::animation::Animation> animation;


      //      ////archive(name, animation);
      //      //archive(name);

      //      //construct(name);


      //      //construct->m_Animation = std::move(animation);



      //  }

      private:
        std::string m_Name;
        Scope<ozz::animation::Animation> m_Animation;
    };

    class AnimationController : public Resource
    {
      public:
        AnimationController() = default;
        AnimationController(std::vector<Animation>&& animations, std::map<std::string, unsigned int>&& animationsMap);

        ~AnimationController() = default;


        void AddAnimation(const std::string& name, std::unique_ptr<ozz::animation::Animation> animation);

        Animation* GetAnimation(const std::string& name);
        Animation* GetAnimation(unsigned int index);

        unsigned int GetAnimationCount() const { return m_Animations.size(); }
        const std::map<std::string, unsigned int>& GetAnimationMap() const { return m_AnimationsMap; }

      //private:
      //  friend class cereal::access;

      //  template <class Archive> void save(Archive& archive) const
      //  {



      //      std::vector<std::string> animationNames;
      //      for (const auto& anim : m_Animations)
      //      {
      //          animationNames.push_back(anim.GetName());
      //      }

      //      archive(animationNames, m_AnimationsMap, cereal::base_class<Resource>(this));
      //  }

      //  template <class Archive> void load(Archive& archive)
      //  {
      //      std::vector<std::string> animationNames;
      //      archive(animationNames, m_AnimationsMap, cereal::base_class<Resource>(this));

      //      m_Animations.clear();
      //      for (const auto& name : animationNames)
      //      {
      //          m_Animations.emplace_back(name);
      //      }
      //  }
      //  template <class Archive>
      //  static void load_and_construct(Archive& archive, cereal::construct<AnimationController>& construct)
      //  {
      //      /*std::vector<std::string> animationNames;
      //      std::map<std::string, unsigned int> animationsMap;

      //      archive(animationNames, animationsMap, cereal::base_class<Resource>(construct.ptr()));

      //      std::vector<Animation> animations;
      //      for (const auto& name : animationNames)
      //      {
      //          animations.emplace_back(name);
      //      }

      //      
      //      construct(std::move(animations), std::move(animationsMap));*/
      //  }


      private:
        std::vector<Animation> m_Animations;
        std::map<std::string, unsigned int> m_AnimationsMap;
    };

} // namespace Coffee

CEREAL_REGISTER_TYPE(Coffee::Animation);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Resource, Coffee::Animation);

CEREAL_REGISTER_TYPE(Coffee::AnimationController);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Resource, Coffee::AnimationController);
