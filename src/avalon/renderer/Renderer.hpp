#pragma once

#include "Shader.hpp"
#include "Camera.hpp"
#include "Color.hpp"

#include "avalon/components/RenderComponent.hpp"
#include "avalon/utils/PlatformUtils.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <freetype/freetype.h>


class QuadRenderBatch {

    static constexpr int positionSize = 3;
    static constexpr int colorSize = 4;
    static constexpr int texCoordsSize = 2;
    static constexpr int texIDSize = 1;

    static constexpr int positionOffset = 0;
    static constexpr int colorOffset = positionOffset + positionSize * sizeof(float);
    static constexpr int texCoordsOffset = colorOffset + colorSize * sizeof(float);
    static constexpr int texIDOffset = texCoordsOffset + texCoordsSize * sizeof(float);

    static constexpr int vertexSize = positionSize + colorSize + texCoordsSize + texIDSize;
    static constexpr int vertexSizeBytes = vertexSize * sizeof(float);

    int32_t maxBatchSize = 0;

    std::vector<float> vertexArray;
    uint32_t index = 0; // holds the drawing index in the element array
    std::vector<uint32_t> elementArray;

    std::vector<Ref<Texture>> textures;
    int texSlots[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    int zIndex;
    bool full = false;

    GLuint VAO{}, VBO{}, EBO{};
    Ref<Shader> shader;

public:

    QuadRenderBatch() = default;


    QuadRenderBatch(int32_t maxBatchSize, Ref<Shader> quadShader, int zIndex) : maxBatchSize(maxBatchSize),
                                                                                shader(std::move(quadShader)),
                                                                                zIndex(zIndex) {
        vertexArray.resize(maxBatchSize * vertexSize * 4); // 4 vertices per quad
        elementArray.resize(maxBatchSize * 6); // 6 indices per quad
    }

    ~QuadRenderBatch() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);

    }

#pragma warning(push)
#pragma warning(disable: 4312)

    void start() {

        // Create and bind the Vertex Array Object (VAO)
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the Vertex Buffer Object (VBO)
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

        // Generate and bind the Element Buffer Object (EBO)
        generateIndices();
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementArray.size() * sizeof(uint32_t), &(elementArray[0]), GL_STATIC_DRAW);

        // bind position on location 0
        glVertexAttribPointer(0, positionSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) positionOffset);
        glEnableVertexAttribArray(0);

        // bind color on location 1
        glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) colorOffset);
        glEnableVertexAttribArray(1);

        // bind color on location 2
        glVertexAttribPointer(2, texCoordsSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) texCoordsOffset);
        glEnableVertexAttribArray(2);

        // bind color on location 3
        glVertexAttribPointer(3, texIDSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) texIDOffset);
        glEnableVertexAttribArray(3);


        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
        glBindVertexArray(0); // Unbind the VAO
    }

#pragma warning(pop)

    /**
     * Adds a quad to draw call. Specify null for non texture quad.
     */
    void addQuad(const glm::vec2 &position, const glm::vec2 scale, const glm::vec4 color, const Ref<Texture> &texture,
                 std::array<glm::vec2, 4> texCoords) {

        int texId = 0;

        if (texture != nullptr) {
            if (std::find(textures.begin(), textures.end(), texture) == textures.end())
                textures.push_back(texture);

            for (int i = 0; i < textures.size(); i++) {
                if (textures[i] == texture) {
                    texId = i + 1;
                    break;
                }
            }
        }

        int offset = index * 4 * vertexSize;


        float xAdd = 0.0f;
        float yAdd = 0.0f;

        for (int i = 0; i < 4; i++) {
            if (i == 1) {
                xAdd = 1.0f;  // Bottom-right corner
                yAdd = 0.0f;
            } else if (i == 2) {
                xAdd = 0.0f;  // Bottom-left corner
                yAdd = 0.0f;
            } else if (i == 3) {
                xAdd = 0.0f;  // Top-left corner
                yAdd = 1.0f;
            } else {
                xAdd = 1.0f;  // Top-right corner
                yAdd = 1.0f;
            }

            // Load position
            vertexArray[offset + 0] = position.x + (xAdd * scale.x);
            vertexArray[offset + 1] = position.y + (yAdd * scale.y);
            vertexArray[offset + 2] = zIndex; // z

            // Load Color
            vertexArray[offset + 3] = color.r;
            vertexArray[offset + 4] = color.g;
            vertexArray[offset + 5] = color.b;
            vertexArray[offset + 6] = color.a;

            // Load texture coordinates
            vertexArray[offset + 7] = texCoords[i].x;
            vertexArray[offset + 8] = texCoords[i].y;

            // Load text id
            vertexArray[offset + 9] = texId;

            offset += vertexSize;
        }
        index++;

        if (index >= maxBatchSize)
            full = true;

    }

    void render(int screenWidth, int screenHeight, Camera &camera) {
        shader->bind();

        camera.applyViewport(screenWidth, screenHeight);

        shader->uploadMat4f("uProjection", camera.getProjectionMatrix());
        shader->uploadMat4f("uView", camera.getViewMatrix());
        shader->uploadFloat("uTime", Time::getTime());


/*        GLint maxViewportDims[2];
        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, maxViewportDims);*/

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i + 1);
            textures[i]->bind();
        }

        shader->uploadIntArray("uTextures", texSlots, 8);

        glBindVertexArray(VAO); // Bind the VAO

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexArray.size() * sizeof(float), &(vertexArray[0]));

        glDrawElements(GL_TRIANGLES, index * 6, GL_UNSIGNED_INT, 0); // Draw the triangles for both quads

        for (int i = 0; i < textures.size(); i++) {
            textures[i]->unbind();
        }

        glBindVertexArray(0); // Unbind the VAO
    }

    bool hasTextureRoom() {
        return textures.size() < 8;
    }

    bool hasTexture(const Ref<Texture> &texture) {
        return std::find(textures.begin(), textures.end(), texture) != textures.end();
    }

    bool isFull() const {
        return full;
    }

    int getZIndex() const {
        return zIndex;
    }

private:
    void generateIndices() {
        /**
         * Vertex 0: Top-right corner.
         * Vertex 1: Bottom-right corner.
         * Vertex 2: Bottom-left corner.
         * Vertex 3: Top-left corner.
         *
         * 3 ---- 0
         * |      |
         * 2------1
         */


        for (int i = 0; i < this->maxBatchSize; i++) {
            // triangle 1 - top left
            elementArray[i * 6 + 0] = i * 4 + 3; // 6 indices, 4 vertices per quad
            elementArray[i * 6 + 1] = i * 4 + 2;
            elementArray[i * 6 + 2] = i * 4 + 0;

            // triangle 2 - below right
            elementArray[i * 6 + 3] = i * 4 + 0;
            elementArray[i * 6 + 4] = i * 4 + 2;
            elementArray[i * 6 + 5] = i * 4 + 1;
        }
    }
};

class TextRenderBatch {

};

class Renderer {
    int32_t maxBatchSize = 0;
    std::vector<QuadRenderBatch> quadBatches;
    glm::vec4 clearColor{1.0f, 1.0f, 1.0f, 1.0f};

    inline static bool initialized = false;

public:
    Renderer() = default;

    Renderer(int32_t maxBatchSize, const glm::vec4 clearColor = {0.0863f, 0.0863f, 0.0863f, 1.0f}) : maxBatchSize(
            maxBatchSize), clearColor(clearColor) {
        if (!initialized) {
            Renderer::init();
            initialized = true;
        }
    }


    void drawQuad(const glm::vec3 &position, const glm::vec2 &scale, const glm::vec4 color,
                  const Sprite &sprite = Sprite(nullptr)) {

        float zIndex = position.z;

        bool added = false;
        for (auto &x: quadBatches) {
            if (!x.isFull() && x.getZIndex() == zIndex) {
                auto &texture = sprite.texture;

                // if quad has no texture
                if (texture == nullptr || (x.hasTexture(texture) || x.hasTextureRoom())) {
                    x.addQuad(position, scale, color, sprite.texture, sprite.texCoords);
                    added = true;
                    break;
                }
            }
        }

        if (!added) {
            quadBatches.emplace_back(maxBatchSize, AssetPool::getBundle("resources")->getShader("quads"), zIndex);
            quadBatches.back().addQuad(position, scale, color, sprite.texture, sprite.texCoords);
        }

    }

    void drawText(const glm::vec3 &position, const glm::ivec2 &size, const glm::vec4 &color, const Font &font,
                  const std::string &text) {

        float zIndex = position.z;

        bool added = false;
    }

    void flush(int screenWidth, int screenHeight, Camera &camera) {

        std::sort(quadBatches.begin(), quadBatches.end(),
                  [](const QuadRenderBatch &a, const QuadRenderBatch &b) {
                      return a.getZIndex() > b.getZIndex();
                  });

        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &batch: quadBatches) {
            batch.start();
            batch.render(screenWidth, screenHeight, camera);
        }
        quadBatches.clear();

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
};

