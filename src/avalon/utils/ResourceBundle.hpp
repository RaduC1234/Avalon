#pragma once

#include "avalon/core/Core.hpp"
#include "avalon/renderer/Texture.hpp"
#include "avalon/renderer/Shader.hpp"
#include "avalon/renderer/Sprite.hpp"
#include "avalon/renderer/Font.hpp"

#include <nlohmann/json.hpp>


class ResourceBundle {

public:
    ResourceBundle(const std::string &directoryPath) {

        for (const auto &entry: std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_directory()) {
                std::string dirName = entry.path().filename().string();
                std::string subDirPath = entry.path().string();

                CreateSwitch<std::string>()
                        .Case("shaders", [this, &subDirPath]() { loadShaders(subDirPath); })
                        .Case("textures", [this, &subDirPath]() { loadTextures(subDirPath); })
                        .Case("spritesheets", [this, &subDirPath]() { loadSprites(subDirPath); })
                                //.Case("fonts", [this, &subDirPath]() { loadFonts(subDirPath); })
                        .Default([]() {})
                        .Execute(dirName);
            }
        }

    }

    Ref<Texture> getTexture(const std::string &filePath) {
        return textures[filePath];
    }

    Sprite getSprite(const std::string& filePath, int index) {
        for (auto &x: sprites) {
            if (x.texture->getFilePath() == filePath && x.index == index) {
                return x;
            }
        }
    }

    Ref<Shader> getShader(const std::string &name) {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

private:

    auto loadSpriteTexture(const std::string &resourceName) {
        auto it = textures.find(resourceName);
        if (it != textures.end()) {
            return it->second;
        } else {
            Ref<Texture> texture = CreateRef<Texture>(resourceName);
            textures[resourceName] = texture;
            return texture;
        }
    }

    void loadShaders(const std::string &directoryPath) {
        for (const auto &entry: std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                std::string shaderName = entry.path().stem().string();
                Ref<Shader> shader = std::make_shared<Shader>(entry.path().string());
                shaders[shaderName] = shader;
            }
        }
    }

    void loadTextures(const std::string &directoryPath) {
        for (const auto &entry: std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                std::string textureName = entry.path().stem().string();
                Ref<Texture> texture = std::make_shared<Texture>(entry.path().string());
                textures[textureName] = texture;
            }
        }
    }

    void loadSprites(const std::string &directoryPath) {
        for (const auto &entry: std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                std::ifstream file(entry.path());
                if (file.is_open()) {

                    try {
                        nlohmann::json jsonData;
                        file >> jsonData;

                        if (!jsonData.contains("type"))
                            continue;

                        std::string resourceType = jsonData["type"];
                        std::string texturePath = jsonData["texture"];

                        if (resourceType == "sprite_sheet") {

                            std::stringstream fullPath;
                            fullPath << directoryPath << "\\" << texturePath;

                            createSpritesFromSheet(
                                    fullPath.str(),
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
                file.close();
            }
        }
    }

    void createSpritesFromSheet(
            std::string textureName,
            float spriteWidth, // pixel per sprite
            float spriteHeight, // pixel per sprite
            float numX, // number of sprites on x-axis
            float numY, // number of sprites on y-axis
            float spacing) { // spacing

        Ref<Texture> texture;
        int spriteIndex = 0;

        try {
            texture = loadSpriteTexture(textureName);
        } catch (const std::exception &exception) {
            AV_CORE_ERROR("Error opening texture: {0}", textureName);
            return;
        }

        int texWidth = texture->getWidth();
        int texHeight = texture->getHeight();

        for (int y = 0; y < numY; y++) {
            for (int x = 0; x < numX; x++) {

                float xOffset = (spriteWidth + spacing) * x;
                float yOffset = (spriteHeight + spacing) * y;

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

                this->sprites.emplace_back(texture, texCoords, spriteIndex++);
            }
        }
    }

    std::unordered_map<std::string, Ref<Texture>> textures;
    std::unordered_map<std::string, Ref<Font>> fonts;
    std::unordered_map<std::string, Ref<Shader>> shaders;
    std::vector<Sprite> sprites;
};
