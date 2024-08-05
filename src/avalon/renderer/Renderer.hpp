#pragma once

#include "avalon/core/Core.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "avalon/logic/Object.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

class QuadRenderBatch {

    static constexpr int positionSize = 3;
    static constexpr int colorSize = 4;

    static constexpr int positionOffset = 0;
    static constexpr int colorOffset = positionOffset + positionSize * sizeof(float);

    static constexpr int vertexSize = positionSize + colorSize;
    static constexpr int vertexSizeBytes = vertexSize * sizeof(float);

    int32_t maxBatchSize = 0;
    Ref<Camera> camera;

    std::vector<float> vertexArray;
    uint32_t index = 0;
    std::vector<uint32_t> elementArray;

    bool full = false;

    GLuint VAO{}, VBO{}, EBO{};
    Shader shader = Shader("resources/shaders/default.glsl");

public:

    QuadRenderBatch() = default;


    QuadRenderBatch(int32_t maxBatchSize, const Ref<Camera> &camera) : maxBatchSize(maxBatchSize), camera(camera) {
        vertexArray.resize(maxBatchSize * vertexSize * 4); // 4 vertices per quad
        elementArray.resize(maxBatchSize * 6); // 6 indices per quad
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
        glVertexAttribPointer(0, positionSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void *) 0);
        glEnableVertexAttribArray(0);

        // bind color on location 1
        glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, vertexSizeBytes,
                              (void *) (positionSize * sizeof(float)));
        glEnableVertexAttribArray(1);


        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
        glBindVertexArray(0); // Unbind the VAO
    }

    void addSprite(const RenderComponent &sprite) {
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
            vertexArray[offset + 0] = sprite.transform.position.x + (xAdd * sprite.transform.scale.x);
            vertexArray[offset + 1] = sprite.transform.position.y + (yAdd * sprite.transform.scale.y);
            vertexArray[offset + 2] = 0; // z

            // Load Color
            vertexArray[offset + 3] = sprite.color.x;
            vertexArray[offset + 4] = sprite.color.y;
            vertexArray[offset + 5] = sprite.color.z;
            vertexArray[offset + 6] = sprite.color.w;

            offset += vertexSize;
        }
        index++;

        if(index >= maxBatchSize)
            full = true;

    }

    void render() {
        shader.bind();

        shader.uploadMat4f("uProjection", camera->getProjectionMatrix());
        shader.uploadMat4f("uView", camera->getViewMatrix());
        shader.uploadFloat("uTime", Time::getTime());

        glBindVertexArray(VAO); // Bind the VAO

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexArray.size() * sizeof(float), &(vertexArray[0]));

        glDrawElements(GL_TRIANGLES, index * 6, GL_UNSIGNED_INT, 0); // Draw the triangles for both quads

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

    void add(RenderComponent& sprite) {
        if (quadBatches.empty() || quadBatches.back().isFull()) {
            quadBatches.emplace_back(maxBatchSize, camera);
            quadBatches.back().start();
        }
        quadBatches.back().addSprite(sprite);
    }

    void render() {
        for (auto& batch : quadBatches) {
            batch.render();
        }
    }

    void flush() {
        quadBatches.clear();
    }

    // helper functions

    void add(Object& object) {
        add(*object.getComponent<RenderComponent>());
    }

    void addAll(std::vector<Object>& objects) {
        for(auto &x : objects)
            add(x);
    }
};
