#pragma once

#include "RenderBatch.hpp"
#include "Font.hpp"

#include <freetype/freetype.h>

// https://learnopengl.com/In-Practice/Text-Rendering
// https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf - implement in shader

class TextRenderBatch : public RenderBatch {

public:

    ~TextRenderBatch() override = default;

    void start() override {

    }

    void render(uint32_t screenWidth, uint32_t screenHeight, Camera &camera) override {

    }

private:

    void addText(const glm::vec2& position, uint32_t size, const glm::vec4& color, const std::string& text, const Font& font, bool normalized) {

    }
};


