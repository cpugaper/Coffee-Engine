#include "Skeleton.h"

namespace Coffee {

    Skeleton::Skeleton(std::vector<Joint> joints, unsigned int numJoints, std::vector<glm::mat4> jointMatrices)
        : Resource(ResourceType::Skeleton),
        m_Joints(std::move(joints)), m_NumJoints(numJoints),
        m_JointMatrices(std::move(jointMatrices))
    {
    }

    void Skeleton::SetSkeleton(ozz::unique_ptr<ozz::animation::Skeleton> skeleton)
    {
        m_Skeleton = std::move(skeleton);
        m_NumJoints = m_Skeleton->num_joints();
        m_JointMatrices.resize(m_NumJoints);
    }

    void Skeleton::SetJoints(const std::vector<Joint>& joints)
    {
        m_Joints = joints;
    }
}