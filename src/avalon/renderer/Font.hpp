#pragma once

#include "avalon/core/Core.hpp"

// https://learnopengl.com/In-Practice/Text-Rendering

struct Character {

    uint32_t textureID; // ID handle of the glyph texture
    glm::ivec2 size; // Size of glyph
    glm::vec2 bearing; // Offset from baseline to left/top of glyph
    uint32_t advance; // Horizontal offset to advance to next glyph
};

class Font {
public:
private:
    std::map<char, Character> characters;
};
