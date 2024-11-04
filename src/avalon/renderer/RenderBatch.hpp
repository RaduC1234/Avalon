#pragma once

#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "avalon/utils/PlatformUtils.hpp"

class RenderBatch {
public:
    RenderBatch() = default;

    RenderBatch(int32_t maxBatchSize, Ref<Shader> quadShader, int zIndex) : maxBatchSize(maxBatchSize), shader(std::move(quadShader)), zIndex(zIndex) {
        vertices.resize(maxBatchSize * 4); // 4 vertices per quad
        indicies.resize(maxBatchSize * 6); // 6 indices per quad
    }

    ~RenderBatch() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
    }


    void start() {
        // Create and bind the Vertex Array Object (VAO)
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the Vertex Buffer Object (VBO)
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        // Generate and bind the Element Buffer Object (EBO)
        generateIndices();
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(uint32_t), &(indicies[0]), GL_STATIC_DRAW);

        // bind position on location 0
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // bind color on location 1
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        // bind texture coordinates on location 2
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);

        // bind texID on location 3
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texID));
        glEnableVertexAttribArray(3);

        // bind normalized flag on location 4
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, isNormalized));
        glEnableVertexAttribArray(4);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
        glBindVertexArray(0); // Unbind the VAO
    }


    void addQuad(const glm::vec2 &position, const glm::vec2 scale, const glm::vec4 color, const Ref<Texture> &texture, std::array<glm::vec2, 4> texCoords, bool normalized = false) {
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

        glm::vec3 positions[4] = {
                {position.x + scale.x, position.y + scale.y, zIndex},
                {position.x + scale.x, position.y,           zIndex},
                {position.x,           position.y,           zIndex},
                {position.x,           position.y + scale.y, zIndex}
        };

        for (int i = 0; i < 4; i++) {
            vertices[vertexIndex * 4 + i] = {
                    positions[i],
                    color,
                    texCoords[i],
                    static_cast<float>(texId),
                    normalized ? 1.0f : 0.0f
            };
        }
        vertexIndex++;

        if (vertexIndex >= maxBatchSize)
            full = true;
    }

    void render(int screenWidth, int screenHeight, Camera &camera) {
        shader->bind();

        camera.applyViewport(screenWidth, screenHeight);
        shader->uploadMat4f("uWorldProjection", camera.getProjectionMatrix());
        shader->uploadMat4f("uView", camera.getViewMatrix());
        shader->uploadFloat("uTime", Time::getTime());

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i + 1);
            textures[i]->bind();
        }

        shader->uploadIntArray("uTextures", texSlots, 8);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

        glDrawElements(GL_TRIANGLES, vertexIndex * 6, GL_UNSIGNED_INT, 0);

        for (int i = 0; i < textures.size(); i++) {
            textures[i]->unbind();
        }

        glBindVertexArray(0);
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
        for (int i = 0; i < this->maxBatchSize; i++) {
            indicies[i * 6 + 0] = i * 4 + 3;
            indicies[i * 6 + 1] = i * 4 + 2;
            indicies[i * 6 + 2] = i * 4 + 0;

            indicies[i * 6 + 3] = i * 4 + 0;
            indicies[i * 6 + 4] = i * 4 + 2;
            indicies[i * 6 + 5] = i * 4 + 1;
        }
    }

    struct Vertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoords;
        float texID;
        float isNormalized;
    };

    int32_t maxBatchSize = 0;
    uint32_t zIndex;
    bool full = false;

    GLuint VAO{}, VBO{}, EBO{};

    std::vector<Vertex> vertices;
    uint32_t vertexIndex = 0; // holds the drawing index in the element array
    std::vector<uint32_t> indicies;

    Ref<Shader> shader;
    std::vector<Ref<Texture>> textures;
    int texSlots[8] = {0, 1, 2, 3, 4, 5, 6, 7};
};
