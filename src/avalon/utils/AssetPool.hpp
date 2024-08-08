#pragma once

#include "avalon/core/Core.hpp"
#include "avalon/renderer/Texture.hpp"
#include "avalon/renderer/Shader.hpp"
#include "avalon/renderer/Sprite.hpp"

#include <nlohmann/json.hpp>

class AssetPool {
public:
    inline static std::unordered_map<std::string, Ref<Shader>> shaders;
    inline static std::unordered_map<std::string, Ref<Texture>> textures;

    inline static std::vector<SpriteSheet> spriteSheets;


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

    inline static Sprite getSprite(const std::string &textureName, int index) {

        for (auto &x: spriteSheets) {
            if(x.texture->getFilePath().ends_with(textureName))
                return x.sprites[index];
        }

        throw std::invalid_argument("Sprite does not exist");
    }


    static void loadIndexedResources() {
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
            std::string textureName,
            float spriteWidth, // pixel per sprite
            float spriteHeight, // pixel per sprite
            float numX, // number of sprites on x-axis
            float numY, // number of sprites on y-axis
            float spacing) { // spacing

        Ref<Texture> texture;
        int spriteIndex = 0;

        try {
            texture = getTexture(textureName);
        } catch (const std::exception &exception) {
            AV_CORE_ERROR("Error opening texture: {0}", textureName);
            return;
        }

        int texWidth = texture->getWidth();
        int texHeight = texture->getHeight();

        SpriteSheet sheet;
        sheet.texture = texture;

        for(int y = 0; y < numY; y++) {
            for(int x = 0; x < numX; x++) {

                // Calculate the position of the sprite within the texture
                float xOffset = (spriteWidth + spacing) * x;
                float yOffset = (spriteHeight + spacing) * y;

                // Calculate normalized texture coordinates
                float texCoordLeft = xOffset / texWidth;
                float texCoordRight = (xOffset + spriteWidth) / texWidth;
                float texCoordBottom = yOffset / texHeight;
                float texCoordTop = (yOffset + spriteHeight) / texHeight;

                std::array<glm::vec2, 4> texCoords = {
                        glm::vec2(texCoordRight, texCoordBottom),  // Bottom-left
                        glm::vec2(texCoordRight, texCoordTop),    // Top-left
                        glm::vec2(texCoordLeft, texCoordTop),   // Top-righta
                        glm::vec2(texCoordLeft, texCoordBottom)// Bottom-right
                };

                // Assuming Sprite is a class that takes a texture, sprite index, and texture coordinates
                sheet.sprites.emplace_back(texture, texCoords, spriteIndex++);
            }
        }

        AssetPool::spriteSheets.push_back(sheet);
    }


};


