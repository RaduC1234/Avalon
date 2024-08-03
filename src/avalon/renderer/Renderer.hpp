#pragma once

#include "avalon/core/Core.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "avalon/logic/Object.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utility>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};


class Renderer {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    GLuint VAO, VBO, EBO;
    std::size_t maxBatchSize;
    std::size_t numVertices;
    bool hasRoom;

    Shader shader;
    Ref<Camera> cameraRef;

public:

    Renderer(std::size_t maxBatchSize, Ref<Camera> cameraRef) : maxBatchSize(maxBatchSize), VAO(0), VBO(0), EBO(0),
    numVertices(0), hasRoom(true),
    cameraRef(std::move(cameraRef)) {
        vertices.reserve(maxBatchSize * 4);
        vertices.reserve(maxBatchSize * 6);

        shader = Shader("resources/shaders/default.glsl");
    }

    ~Renderer() {

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
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Vertex), indices.data(), GL_STATIC_DRAW);

        const int positionSize = 3;
        const int colorSize = 4;

        // bind position on location 0
        glVertexAttribPointer(0, positionSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
    }

    void addObjectToDraw(RenderComponent* object) {

    }
};
