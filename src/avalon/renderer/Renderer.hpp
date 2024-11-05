#pragma once

#include "RenderBatch.hpp"
#include "avalon/utils/AssetPool.hpp"

enum Shape : uint32_t {
    QUAD,
    CIRCLE
};

class Renderer {
public:
    Renderer() = default;

    Renderer(int32_t maxBatchSize, const glm::vec4 clearColor = {0.0863f, 0.0863f, 0.0863f, 1.0f}) : maxBatchSize(maxBatchSize), clearColor(clearColor) {
        if (!initialized) {
            Renderer::init();
            initialized = true;
        }
    }


    void draw(const glm::vec3 &position, const glm::vec2 &scale, float rotation, Shape shape, const glm::vec4 color, const Ref<Texture> &texture, const TextureCoords &texCoords) {

        float zIndex = position.z;

        bool added = false;
        for (auto &x: batches) {
            if (!x.isFull() && x.getZIndex() == zIndex) {

                // if quad has no texture
                if (texture == nullptr || (x.hasTexture(texture) || x.hasTextureRoom())) {
                    x.addShape(position, scale, rotation, shape, color, texture, texCoords);
                    added = true;
                    break;
                }
            }
        }

        if (!added) {
            batches.emplace_back(maxBatchSize, AssetPool::getBundle("resources")->getShader("render"), zIndex);
            batches.back().addShape(position, scale, rotation, shape, color, texture, texCoords);
        }

    }

    void drawQuad(const glm::vec3 &position, const glm::vec2 size, const glm::vec4 &color, const Sprite& sprite = Sprite(nullptr)) {
        draw(position, size, 0.0f, Shape::QUAD, color, sprite.texture, sprite.texCoords);
    }

    void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 size, float rotation, const glm::vec4 &color, const Sprite& sprite = Sprite(nullptr)) {
        draw(position, size, rotation, Shape::QUAD, color, sprite.texture, sprite.texCoords);
    }

    void drawText(const glm::vec3 &position, const glm::ivec2 &size, const glm::vec4 &color, const Font &font, const std::string &text, bool normalized = false) {

        float zIndex = position.z;

        bool added = false;
    }

    void flush(int screenWidth, int screenHeight, Camera &camera) {

        std::sort(batches.begin(), batches.end(),
                  [](const RenderBatch &a, const RenderBatch &b) {
                      return a.getZIndex() > b.getZIndex();
                  });

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &batch: batches) {
            batch.start();
            batch.render(screenWidth, screenHeight, camera);
        }
        batches.clear();

        GLenum err;
        if ((err = glGetError()) != GL_NO_ERROR) {
            AV_CORE_ERROR("OpenGL error: {0}", err);
        }
    }

    void static init() {

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(1);
        }

        int textureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
        AV_CORE_INFO("Texture units available on hardware: {0}.", textureUnits);

        glDisable(GL_DEPTH_TEST);
        // enable transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Enable Anti-Aliasing - todo: implement with framebuffer - also check window class when removing this, line 40
        glEnable(GL_MULTISAMPLE);

        // Initialize FreeType
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }

        AssetPool::loadBundle("resources");
    }

private:
    int32_t maxBatchSize = 0;
    std::vector<RenderBatch> batches;
    glm::vec4 clearColor{1.0f, 1.0f, 1.0f, 1.0f};

    inline static bool initialized = false;
};
