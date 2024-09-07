#pragma once

#include "avalon/core/Core.hpp"
#include "Scene.hpp"

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

class SceneSerializer
{
public:
    SceneSerializer(const Ref<Scene>& scene) : scene(scene) {}

    void Serialize(const std::string& filepath) {

    }

    bool Deserialize(const std::string& filepath) {
        return false;
    }

private:
    Ref<Scene> scene;
};
