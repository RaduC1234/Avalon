#pragma once

#include "RenderBatch.hpp"
#include "Font.hpp"

// https://learnopengl.com/In-Practice/Text-Rendering
// https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf - implement in shader

class TextRenderBatch : public RenderBatch {
    static constexpr int positionSize = 2;  // (x, y)
    static constexpr int texCoordsSize = 2; // (s, t)

    static constexpr int vertexSize = positionSize + texCoordsSize;
    static constexpr int vertexSizeBytes = vertexSize * sizeof(float);

public:

    TextRenderBatch(int32_t maxBatchSize, Ref<Shader> textShader, uint32_t zIndex) : RenderBatch(maxBatchSize, zIndex, RenderType::TEXT), shader(std::move(textShader)) {
        vertexArray.resize(maxBatchSize * vertexSize * 4);  // 4 vertices per glyph quad
        elementArray.resize(maxBatchSize * 6);  // 6 indices per glyph quad (2 triangles)
    }

    ~TextRenderBatch() override {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
    }

    void start() override {
        // Create and bind the Vertex Array Object (VAO)
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Create and bind VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

        // Create and bind EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        generateIndices();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementArray.size() * sizeof(uint32_t), &elementArray[0], GL_STATIC_DRAW);

        // Specify vertex attributes
        glVertexAttribPointer(0, positionSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, texCoordsSize, GL_FLOAT, GL_FALSE, vertexSizeBytes, (void*)(positionSize * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Unbind VAO
        glBindVertexArray(0);
    }

    void render(uint32_t screenWidth, uint32_t screenHeight, Camera &camera) override {

    }


    void addText(const glm::vec2& position, uint32_t size, const glm::vec4& color, const std::string& text, const Font& font, bool normalized) {

    }

    Font getFont() {
        return Font();
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

    std::vector<float> vertexArray;
    uint32_t index = 0;
    std::vector<uint32_t> elementArray;

    GLuint VAO{}, VBO{}, EBO{};
    Ref<Shader> shader;
};


