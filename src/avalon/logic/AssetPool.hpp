#pragma once

#include "avalon/core/Core.hpp"
#include "avalon/renderer/Texture.hpp"
#include "avalon/renderer/Shader.hpp"

class AssetPool {
    inline static std::unordered_map<std::string, Ref<Shader>> shaders;
    inline static std::unordered_map<std::string, Ref<Texture>> textures;

public:
    inline static Ref<Shader> getShader(const std::string& resourceName) {
        auto it = shaders.find(resourceName);
        if (it != shaders.end()) {
            return it->second;
        } else {
            Ref<Shader> shader = CreateRef<Shader>("resources/shaders/" + resourceName);
            shaders[resourceName] = shader;
            return shader;
        }
    }

    inline static Ref<Texture> getTexture(const std::string& resourceName) {
        auto it = textures.find(resourceName);
        if (it != textures.end()) {
            return it->second;
        } else {
            Ref<Texture> texture = CreateRef<Texture>("resources/textures/" + resourceName);
            textures[resourceName] = texture;
            return texture;
        }
    }

    inline static Ref<Texture> DEFAULT_TEXTURE() {
        return getTexture("default.png");
    }
};


