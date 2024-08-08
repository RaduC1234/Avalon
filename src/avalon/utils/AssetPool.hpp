#pragma once

#include "avalon/core/Core.hpp"
#include "avalon/renderer/Texture.hpp"
#include "avalon/renderer/Shader.hpp"
#include "avalon/renderer/Sprite.hpp"

#include <nlohmann/json.hpp>

class AssetPool {
    inline static std::unordered_map<std::string, Ref<Shader>> shaders;
    inline static std::unordered_map<std::string, Ref<Texture>> textures;

    inline static std::vector<Sprite> sprites;

public:
    inline static Ref<Shader> getShader(const std::string &resourceName) {
        auto it = shaders.find(resourceName);
        if (it != shaders.end()) {
            return it->second;
        } else {
            Ref<Shader> shader = CreateRef<Shader>("resources/shaders/" + resourceName);
            shaders[resourceName] = shader;
            return shader;
        }
    }

    inline static Ref<Texture> getTexture(const std::string &resourceName) {
        auto it = textures.find(resourceName);
        if (it != textures.end()) {
            return it->second;
        } else {
            Ref<Texture> texture = CreateRef<Texture>("resources/textures/" + resourceName);
            textures[resourceName] = texture;
            return texture;
        }
    }

    inline static Sprite getSprite(const std::string &texturePath, int index) {
        for (auto &x: sprites) {
            if (x.texture->getFilePath() == texturePath && x.index == index) {
                return x;
            }
        }

        return Sprite();
    }

    static void init() {
        const std::string resourcePack = "resources/textures";

        for (const auto &entry: std::filesystem::directory_iterator(resourcePack)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                std::ifstream file(entry.path());
                if (file.is_open()) {

                    try {
                        nlohmann::json jsonData;
                        file >> jsonData;

                        if (!jsonData.contains("type"))
                            continue;

                        std::string resourceType = jsonData["type"];

                        if (resourceType == "sprite_sheet") {

                            createSpritesFromSheet(
                                    jsonData["texture"],
                                    jsonData["sprite_width"],
                                    jsonData["sprite_height"],
                                    jsonData["num_x"],
                                    jsonData["num_y"],
                                    jsonData["spacing"]
                            );
                        }

                    } catch (const nlohmann::json::parse_error &e) {
                        std::stringstream ss;
                        ss << "JSON parse error in file " << entry.path() << ": " << e.what();
                        AV_CORE_WARN(ss.str());
                    } catch (const std::exception &e) {
                        std::stringstream ss;
                        ss << "Error deserializing file " << entry.path() << ": " << e.what();
                        AV_CORE_WARN(ss.str());
                    }
                }
            }
        }
    }

    inline static Ref<Texture> DEFAULT_TEXTURE() {
        return getTexture("default.png");
    }

private:
    static void createSpritesFromSheet(
            std::string texturePath,
            int spriteWidth,
            int spriteHeight,
            int numX,
            int numY,
            int spacing) {

        //auto texture = getTexture(texturePath);


    }
};


