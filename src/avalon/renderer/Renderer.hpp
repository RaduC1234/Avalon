#pragma once

#include "Shader.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Color.hpp"

#include "avalon/logic/Object.hpp"
#include "avalon/utils/Time.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

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
    Ref<Camera> camera;

    std::vector<float> vertexArray;
    uint32_t index = 0;
    std::vector<uint32_t> elementArray;

    std::vector<Ref<Texture>> textures;
    int texSlots[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    bool full = false;

    GLuint VAO{}, VBO{}, EBO{};
    Ref<Shader> shader = AssetPool::getShader("quads.glsl");

public:

    QuadRenderBatch() = default;


    QuadRenderBatch(int32_t maxBatchSize, const Ref<Camera> &camera) : maxBatchSize(maxBatchSize), camera(camera) {
        vertexArray.resize(maxBatchSize * vertexSize * 4); // 4 vertices per quad
        elementArray.resize(maxBatchSize * 6); // 6 indices per quad
    }

    ~QuadRenderBatch() {
        if(VAO) glDeleteVertexArrays(1, &VAO);
        if(VBO) glDeleteBuffers(1, &VBO);
        if(EBO) glDeleteBuffers(1, &EBO);

    }


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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementArray.size() * sizeof(uint32_t), &(elementArray[0]),
                     GL_STATIC_DRAW);

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

    void addSprite(const RenderComponent &renderComponent) {

        int texId = 0;
        Ref<Texture> texture = renderComponent.spriteRef->texture;

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
            vertexArray[offset + 0] = renderComponent.transform.position.x + (xAdd * renderComponent.transform.scale.x);
            vertexArray[offset + 1] = renderComponent.transform.position.y + (yAdd * renderComponent.transform.scale.y);
            vertexArray[offset + 2] = 0; // z

            // Load Color
            vertexArray[offset + 3] = renderComponent.color.r;
            vertexArray[offset + 4] = renderComponent.color.g;
            vertexArray[offset + 5] = renderComponent.color.b;
            vertexArray[offset + 6] = renderComponent.color.a;

            // Load texture coordinates
            vertexArray[offset + 7] = renderComponent.spriteRef->texCoords[i].x;
            vertexArray[offset + 8] = renderComponent.spriteRef->texCoords[i].y;

            // Load text id
            vertexArray[offset + 9] = texId;

            offset += vertexSize;
        }
        index++;

        if (index >= maxBatchSize)
            full = true;

    }

    void render() {
        shader->bind();

        shader->uploadMat4f("uProjection", camera->getProjectionMatrix());
        shader->uploadMat4f("uView", camera->getViewMatrix());
        shader->uploadFloat("uTime", Time::getTime());

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i + 1);
            textures[i]->bind();
        }

        shader->uploadIntArray("uTextures", texSlots);

        glBindVertexArray(VAO); // Bind the VAO

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexArray.size() * sizeof(float), &(vertexArray[0]));

        glDrawElements(GL_TRIANGLES, index * 6, GL_UNSIGNED_INT, 0); // Draw the triangles for both quads

        for(int i = 0; i < textures.size(); i++) {
            textures[i]->unbind();
        }

        glBindVertexArray(0); // Unbind the VAO
    }

    bool isFull() {
        return full;
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

class Renderer {
    int32_t maxBatchSize = 0;
    Ref<Camera> camera;
    std::vector<QuadRenderBatch> quadBatches;

public:
    Renderer() = default;

    Renderer(int32_t maxBatchSize, const Ref<Camera> &camera) : maxBatchSize(maxBatchSize), camera(camera) {}

    void add(const RenderComponent &renderComponent) {

        if(!renderComponent.isVisible)
            return;

        if (quadBatches.empty() || quadBatches.back().isFull()) {
            quadBatches.emplace_back(maxBatchSize, camera);
            quadBatches.back().start();
        }
        quadBatches.back().addSprite(renderComponent);
    }

    void render() {
        for (auto &batch: quadBatches) {
            batch.render();
        }
    }

    void flush() {
        quadBatches.clear();
    }

    // helper functions

    void add(Object &object) {
        add(*object.getComponent<RenderComponent>());
    }

    void addAll(std::vector<Object> &objects) {
        for (auto &x: objects)
            add(x);
    }
};

