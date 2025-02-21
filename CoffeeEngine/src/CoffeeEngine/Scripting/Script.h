#pragma once

#include <cereal/external/rapidjson/reader.h>
#include <entt/entity/registry.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include "CoffeeEngine/IO/Serialization/FilesystemPathSerialization.h"

#include <filesystem>
#include <functional>
#include <any>
#include <unordered_map>
#include <cereal/types/polymorphic.hpp>


namespace Coffee
{

    enum class ScriptingLanguage;

    enum class ExportedVariableType
    {
        None,
        Int,
        Float,
        String,
        Bool,
        Vector2,
        Vector3,
        Vector4,
        Matrix3,
        Matrix4,
        Entity,
        Scene,
        Material,
        Light
    };

    struct ExportedVariable
    {
        public:
            std::string name;
            std::any value;
            ExportedVariableType type;

        private:
            friend class cereal::access;

            template<class Archive>
            void serialize(Archive& archive) const
            {
                archive(
                    cereal::make_nvp("name", name),
                    cereal::make_nvp("value", value),
                    cereal::make_nvp("type", type)
                );
            }
    };

    class Script
    {
    public:
        // Script() {}
        Script(const std::filesystem::path& path) : m_Path(path) {}
        virtual ~Script() = default;
        virtual void OnReady() = 0;
        virtual void OnUpdate(float dt) = 0;
        virtual void OnExit() = 0;

        // The function parameter should be more generic
        template <typename Ret, typename... Args>
        void RegisterFunction(const std::string& name, std::function<Ret(Args...)> function);
        // TODO: Add support for variadic arguments
        virtual void CallFunction(const std::string& functionName) = 0;

        // The variable parameter should be more generic
        //virtual void SetVariable(const std::string& name, std::any value) = 0;
        template <typename T>
        void SetVariable(const std::string& name, T value);

        template <typename T>
        T GetVariable(const std::string& name);

        virtual void ParseScript() = 0;

        std::filesystem::path GetPath() const { return m_Path; }

        bool HasPath() const { return !m_Path.empty(); }

        inline std::unordered_map<std::string, ExportedVariable>& GetExportedVariables() { return m_ExportedVariables; }

    private:
        friend class cereal::access;

        template<class Archive>
        void save(Archive& archive) const
        {
            archive(
                cereal::make_nvp("path", m_Path),
                cereal::make_nvp("language", m_Language),
                cereal::make_nvp("exported_variables", m_ExportedVariables)
            );
        }

        template<class Archive>
        void load(Archive& archive)
        {
            archive(
                cereal::make_nvp("path", m_Path),
                cereal::make_nvp("language", m_Language),
                cereal::make_nvp("exported_variables", m_ExportedVariables)
            );
        }

    protected:
        ScriptingLanguage m_Language;
        std::filesystem::path m_Path;
        std::unordered_map<std::string, ExportedVariable> m_ExportedVariables;
    };

}

CEREAL_REGISTER_TYPE(Coffee::Script);
