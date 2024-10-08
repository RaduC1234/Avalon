#pragma once

#include "avalon/utils/PlatformUtils.hpp"
#include "RenderBatch.hpp"


class QuadRenderBatch : public RenderBatch {

    static constexpr int positionSize = 3;
    static constexpr int colorSize = 4;
    static constexpr int texCoordsSize = 2;
    static constexpr int texIDSize = 1;
    static constexpr int isNormalizedFlagSize = 1;

    static constexpr int positionOffset = 0;
    static constexpr int colorOffset = positionOffset + positionSize * sizeof(float);
    static constexpr int texCoordsOffset = colorOffset + colorSize * sizeof(float);
    static constexpr int texIDOffset = texCoordsOffset + texCoordsSize * sizeof(float);
    static constexpr int isNormalizedOffset = texIDOffset + texIDSize * sizeof(float);


    static constexpr int vertexSize = positionSize + colorSize + texCoordsSize + texIDSize + isNormalizedFlagSize;
    static constexpr int vertexSizeBytes = vertexSize * sizeof(float);


    std::vector<float> vertexArray;
    uint32_t index = 0; // holds the drawing index in the element array
    std::vector<uint32_t> elementArray;

    std::vector<Ref<Texture>> textures;
    int texSlots[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    bool full = false;

    GLuint VAO{}, VBO{}, EBO{};
    Ref<Shader> shader;

public:

    QuadRenderBatch(int32_t maxBatchSize, Ref<Shader> quadShader, uint32_t zIndex) : RenderBatch(maxBatchSize, zIndex, RenderType::QUAD), shader(std::move(quadShader)) {
        vertexArray.resize(maxBatchSize * vertexSize * 4); // 4 vertices per quad
        elementArray.resize(maxBatchSize * 6); // 6 indices per quad
    }

    ~QuadRenderBatch() override {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);

    }

#pragma warning(push)
#pragma warning(disable: 4312)

    void start() override {

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

        // bind texID on location 3
        glVertexAttribPointer(3, texIDSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) texIDOffset);
        glEnableVertexAttribArray(3);

        // bind normalized flag on location 4
        glVertexAttribPointer(4, isNormalizedFlagSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) isNormalizedOffset);
        glEnableVertexAttribArray(4);


        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
        glBindVertexArray(0); // Unbind the VAO
    }

#pragma warning(pop)

    /**
     * Adds a quad to draw call. Specify null for non texture quad.
     */
    void addQuad(const glm::vec2 &position, const glm::vec2 &scale, const glm::vec4 &color, const Ref<Texture> &texture, TextureCoords texCoords, bool normalized = false) {
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
            vertexArray[offset + 2] = getZIndex(); // z

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

            // Load isNormalized Flag
            vertexArray[offset + 10] = normalized ? 1.0f : 0.0f;

            offset += vertexSize;
        }

        index++;

        if (index >= maxBatchSize)
            full = true;

    }

    void render(uint32_t screenWidth, uint32_t screenHeight, Camera &camera) override {
        shader->bind();

        camera.applyViewport(screenWidth, screenHeight);

        glm::mat4 uiProjectionMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(screenWidth, screenHeight, 1.0f));

        shader->uploadMat4f("uWorldProjection", camera.getProjectionMatrix());
        shader->uploadMat4f("uView", camera.getViewMatrix());
        shader->uploadMat4f("uNormalizedProjection", uiProjectionMatrix);
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

private:
    constexpr void generateIndices() {
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
