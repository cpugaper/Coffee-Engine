#pragma once

#include <ozz/animation/runtime/skeleton.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/memory/unique_ptr.h>
#include <glm/mat4x4.hpp>

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>




#include <string>
#include <vector>

namespace ozz::math
{
    template <class Archive> void serialize(Archive& archive, Float3& vec)
    {
        archive(vec.x, vec.y, vec.z);
    }

    template <class Archive> void serialize(Archive& archive, Quaternion& quat)
    {
        archive(quat.x, quat.y, quat.z, quat.w);
    }
}


namespace Coffee {




struct Joint
    {
        std::string name;
        int parentIndex;
        ozz::math::Transform localTransform;
        glm::mat4 invBindPose;

      private:
        friend class cereal::access;

        template <class Archive> void serialize(Archive& archive)
        {
            archive(name, parentIndex, localTransform.translation, localTransform.rotation, localTransform.scale,
                    invBindPose);
        }
    };


    class Skeleton : public Resource
    {
    public:
        Skeleton() = default;
        Skeleton(std::vector<Joint> joints, unsigned int numJoints, std::vector<glm::mat4> jointMatrices);

        ~Skeleton() = default;

        void SetSkeleton(ozz::unique_ptr<ozz::animation::Skeleton> skeleton);
        const ozz::animation::Skeleton* GetSkeleton() const { return m_Skeleton.get(); }

        void SetJoints(const std::vector<Joint>& joints);
        const std::vector<Joint>& GetJoints() const { return m_Joints; }

        unsigned int GetNumJoints() const { return m_NumJoints; }
        const std::vector<glm::mat4>& GetJointMatrices() const { return m_JointMatrices; }


    private:
        friend class cereal::access;

        template <class Archive> void save(Archive& archive) const
        {

            archive(m_Joints, m_NumJoints, m_JointMatrices, cereal::base_class<Resource>(this));
        }

        template <class Archive> void load(Archive& archive)
        {

            archive(m_Joints, m_NumJoints, m_JointMatrices, cereal::base_class<Resource>(this));
        }

        template <class Archive> static void load_and_construct(Archive& data, cereal::construct<Skeleton>& construct)
        {


            std::vector<Joint> joints;
            unsigned int numJoints;
            std::vector<glm::mat4> jointMatrices;

            data(joints, numJoints, jointMatrices, cereal::base_class<Resource>(construct.ptr()));

            construct(joints, numJoints, jointMatrices);

        }



    private:
        ozz::unique_ptr<ozz::animation::Skeleton> m_Skeleton;
        std::vector<Joint> m_Joints;
        unsigned int m_NumJoints = 0;
        std::vector<glm::mat4> m_JointMatrices;
    };

} // Coffee
CEREAL_REGISTER_TYPE(Coffee::Skeleton);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Resource, Coffee::Skeleton);
