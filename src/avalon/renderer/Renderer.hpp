#pragma once

#include "avalon/core/Core.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "avalon/logic/Object.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

class QuadRenderBatch {

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
    };

    int VERTEX_SIZE = 7;

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    GLuint VAO, VBO, EBO;
    std::size_t maxBatchSize;
    std::size_t sprites = 0;
    bool b_hasRoom = true;

    static const auto verticesPerQuad = 4;
    static const auto indicesPerQuad = 6;

    Shader shader = Shader("resources/shaders/default.glsl");
    Ref<Camera> cameraRef;

public:

    QuadRenderBatch(std::size_t maxBatchSize, Ref<Camera> cameraRef) : maxBatchSize(maxBatchSize), VAO(0), VBO(0),
                                                                       EBO(0),
                                                                       cameraRef(std::move(cameraRef)) {
        vertices.resize(maxBatchSize * verticesPerQuad * VERTEX_SIZE); // 4 vertices per quad

    }

    ~QuadRenderBatch() {

        //if (VAO) glDeleteVertexArrays(1, &VAO);
        //if (VBO) glDeleteBuffers(1, &VBO);
        //if (EBO) glDeleteBuffers(1, &EBO);
    }

    static constexpr float vertexArray[] = {
            // First quad vertices
            // position              // color
            400.0f, 0.0f, 0.0f,     1.0f, 0.0f, 1.0f, 1.0f, // bottom right
            0.0f, 400.0f, 0.0f,     1.0f, 0.0f, 1.0f, 1.0f, // top left
            400.0f, 400.0f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f, // top right
            0.0f, 0.0f, 0.0f,       1.0f, 0.0f, 1.0f, 1.0f, // bottom left

            // Second quad vertices
            // position              // color
            1200.0f, 0.0f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f, // bottom right
            800.0f, 400.0f, 0.0f,    0.0f, 1.0f, 1.0f, 1.0f, // top left
            1200.0f, 400.0f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f, // top right
            800.0f, 0.0f, 0.0f,      0.0f, 1.0f, 1.0f, 1.0f  // bottom left
    };

    // must be in counter-clockwise order
    static constexpr int elementArray[] = {
            // First quad indices
            2, 1, 0,
            0, 1, 3,

            // Second quad indices (adjusted for the second set of vertices)
            6, 5, 4,
            4, 5, 7
    };

    void start() {

        // Create and bind the Vertex Array Object (VAO)
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the Vertex Buffer Object (VBO)
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

        // Generate and bind the Element Buffer Object (EBO)
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArray), elementArray, GL_STATIC_DRAW);

        const int positionSize = 3;
        const int colorSize = 4;
        int vertexSizeBytes = (positionSize + colorSize) * sizeof(float);

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

    // for now everything is a quad
    void addSprite(RenderComponent &sprite) {

        int offset = sprites * verticesPerQuad * VERTEX_SIZE;

        auto color = sprite.color;

        float xAdd = 0.0f;
        float yAdd = 0.0f;
        for (int i = 0; i < 4; i++) {
            if (i == 1) {
                xAdd = 1.0f;  // Bottom-right corner
                yAdd = 0.0f;
            } else if (i == 2) {
                xAdd = 1.0f;  // Bottom-left corner
                yAdd = 1.0f;
            } else if (i == 3) {
                xAdd = 0.0f;  // Top-left corner
                yAdd = 1.0f;
            } else {
                xAdd = 0.0f;  // Top-right corner
                yAdd = 0.0f;
            }

            // Load position
            vertices[offset] = sprite.transform.getPosition().x + (xAdd * sprite.transform.getScale().x);
            vertices[offset + 1] = sprite.transform.getPosition().y + (yAdd * sprite.transform.getScale().y);
            vertices[offset + 2] = 0.0f; // z

            // Load color
            vertices[offset + 3] = color.x;
            vertices[offset + 4] = color.y;
            vertices[offset + 5] = color.z;
            vertices[offset + 6] = color.w;

            offset += VERTEX_SIZE;

            sprites++;
        }
    }


    void render() {

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
       // glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

       shader.bind();

        shader.uploadMat4f("uProjection", cameraRef->getProjectionMatrix());
        shader.uploadMat4f("uView", cameraRef->getViewMatrix());
        shader.uploadFloat("uTime", Time::getTime());

        glBindVertexArray(VAO); // Bind the VAO

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); // Draw the triangles for both quads

        glBindVertexArray(0); // Unbind the VAO
    }

    bool hasRoom() const {
        return this->b_hasRoom;
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

        indices.resize(maxBatchSize * indicesPerQuad); // 6 indices per quad ( 3 per triangle )

        for (int i = 0; i < this->maxBatchSize; i++) {
            // triangle 1 - top left
            indices[i * indicesPerQuad + 0] = i * verticesPerQuad + 2;
            indices[i * indicesPerQuad + 1] = i * verticesPerQuad + 3;
            indices[i * indicesPerQuad + 2] = i * verticesPerQuad + 0;

            // triangle 2 - below right
            indices[i * indicesPerQuad + 3] = i * verticesPerQuad + 2;
            indices[i * indicesPerQuad + 4] = i * verticesPerQuad + 0;
            indices[i * indicesPerQuad + 5] = i * verticesPerQuad + 1;
        }
    }

};

class Renderer {

    int32_t maxBatchSize = 0;
    Ref<Camera> camera;


    GLuint VAO{}, VBO{}, EBO{};
    Shader shader = Shader("resources/shaders/default.glsl");
    static constexpr float vertexArray[] = {
            // First quad vertices
            // position              // color
            400.0f, 0.0f, 0.0f,     1.0f, 0.0f, 1.0f, 1.0f, // bottom right
            0.0f, 400.0f, 0.0f,     1.0f, 0.0f, 1.0f, 1.0f, // top left
            400.0f, 400.0f, 0.0f,   1.0f, 0.0f, 1.0f, 1.0f, // top right
            0.0f, 0.0f, 0.0f,       1.0f, 0.0f, 1.0f, 1.0f, // bottom left

            // Second quad vertices
            // position              // color
            1200.0f, 0.0f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f, // bottom right
            800.0f, 400.0f, 0.0f,    0.0f, 1.0f, 1.0f, 1.0f, // top left
            1200.0f, 400.0f, 0.0f,   0.0f, 1.0f, 1.0f, 1.0f, // top right
            800.0f, 0.0f, 0.0f,      0.0f, 1.0f, 1.0f, 1.0f  // bottom left
    };

    // must be in counter-clockwise order
    static constexpr int elementArray[] = {
            // First quad indices
            2, 1, 0,
            0, 1, 3,

            // Second quad indices (adjusted for the second set of vertices)
            6, 5, 4,
            4, 5, 7
    };

public:
    Renderer() = default;

    Renderer(int32_t maxBatchSize, const Ref<Camera> &camera) : maxBatchSize(maxBatchSize), camera(camera) {}

   /* void registerObjects(const std::vector<Object> &objects) {

        for (auto &x: objects) {
            RenderComponent renderComp = *x.getComponent<RenderComponent>();

            switch (renderComp.shape) {

                case RenderComponent::Shape::QUAD:
                    bool added = false;

                    for (auto &quadBatch: quads) {
                        if (quadBatch.hasRoom()) {
                            quadBatch.addSprite(renderComp);
                            added = true;
                            break;
                        }
                    }

                    if(!added) {
                        QuadRenderBatch newBatch(1, camera);
                        newBatch.start();
                        newBatch.addSprite(renderComp);
                        quads.push_back(std::move(newBatch));
                    }
                    break;
            }
        }
    }
*/
    void start() {
        // Create and bind the Vertex Array Object (VAO)
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the Vertex Buffer Object (VBO)
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

        // Generate and bind the Element Buffer Object (EBO)
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementArray), elementArray, GL_STATIC_DRAW);

        const int positionSize = 3;
        const int colorSize = 4;
        int vertexSizeBytes = (positionSize + colorSize) * sizeof(float);

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

    void render() {
        shader.bind();

        shader.uploadMat4f("uProjection", camera->getProjectionMatrix());
        shader.uploadMat4f("uView", camera->getViewMatrix());
        shader.uploadFloat("uTime", Time::getTime());

        glBindVertexArray(VAO); // Bind the VAO

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); // Draw the triangles for both quads

        glBindVertexArray(0); // Unbind the VAO
    }

    void flush() {
        //quads.clear();
    }

};
