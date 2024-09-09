#pragma once

#include "QuadRenderBatch.hpp"
#include "TextRenderBatch.hpp"
#include "Sprite.hpp"

#include "avalon/utils/AssetPool.hpp"

class Renderer {
public:
    Renderer() = default;

    Renderer(int32_t maxBatchSize, const glm::vec4 clearColor = {0.0863f, 0.0863f, 0.0863f, 1.0f}) : maxBatchSize(maxBatchSize), clearColor(clearColor) {
        if (!initialized) {
            Renderer::init();
            initialized = true;
        }
    }

    void drawNormalizedQuad(const glm::vec3 &position, const glm::vec2 &scale, const glm::vec4 color, const Sprite &sprite = Sprite(nullptr)) {
        drawQuad(position, scale, color, sprite, true);
    }

    void drawQuad(const glm::vec3 &position, const glm::vec2 &scale, const glm::vec4 color, const Sprite &sprite = Sprite(nullptr)) {
        drawQuad(position, scale, color, sprite, false);
    }

    void drawText(const glm::vec3 &position, const glm::ivec2 &size, const glm::vec4 &color, const Font &font, const std::string &text, bool normalized = false) {

        float zIndex = position.z;

        bool added = false;
    }

    void flush(int screenWidth, int screenHeight, Camera &camera) {

        std::sort(renderBatches.begin(), renderBatches.end(),
                  [](Scope<RenderBatch> &a, Scope<RenderBatch> &b) {
                      return a->getZIndex() > b->getZIndex();
                  });

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &batch: renderBatches) {
            batch->start();
            batch->render(screenWidth, screenHeight, camera);
        }
        renderBatches.clear();

        GLenum err;
        if ((err = glGetError()) != GL_NO_ERROR) {
            AV_CORE_ERROR("OpenGL error: {0}", err);
        }
    }

private:

// main function
    void drawQuad(const glm::vec3 &position, const glm::vec2 &scale, const glm::vec4 &color, const Sprite &sprite, bool normalized) {
        float zIndex = position.z;
        bool added = false;


        for (auto &batch: renderBatches) {

            if (batch->getBatchType() != RenderType::QUAD)
                continue;


            auto *quadBatch = dynamic_cast<QuadRenderBatch *>(batch.get());
            if (quadBatch && !quadBatch->isFull() && quadBatch->getZIndex() == zIndex) {
                auto &texture = sprite.texture;


                if (!texture || quadBatch->hasTexture(texture) || quadBatch->hasTextureRoom()) {
                    quadBatch->addQuad(position, scale, color, texture, sprite.texCoords, normalized);
                    added = true;
                    break;
                }
            }
        }

        if (!added) {

            auto quadBatch = CreateScope<QuadRenderBatch>(
                    maxBatchSize,
                    AssetPool::getBundle("resources")->getShader("quads"),
                    zIndex
            );
            quadBatch->addQuad(position, scale, color, sprite.texture, sprite.texCoords, normalized);

            renderBatches.push_back(std::unique_ptr<RenderBatch>(std::move(quadBatch)));
        }
    }


    void static init() {

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(1);
        }

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
        AV_CORE_INFO("Texture units available on hardware: {0}.", maxTextureSlots);

        glDisable(GL_DEPTH_TEST);

        // enable transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // nu stiu ce face asta dar asa scrie in documentatie
        glEnable(GL_CULL_FACE);

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

    int32_t maxBatchSize = 0;
    glm::vec4 clearColor{1.0f, 1.0f, 1.0f, 1.0f};

    std::vector<Scope<RenderBatch>> renderBatches;

    inline static bool initialized = false;
    inline static int maxTextureSlots = 0;
};

