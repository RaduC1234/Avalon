#pragma once

#include "avalon/core/Core.hpp"

#include <freetype/freetype.h>

// https://learnopengl.com/In-Practice/Text-Rendering

struct Character {

    uint32_t textureID; // ID handle of the glyph texture
    glm::ivec2 size; // Size of glyph
    glm::vec2 bearing; // Offset from baseline to left/top of glyph
    uint32_t advance; // Horizontal offset to advance to next glyph
};

class Font {
public:
    Font() = default;

    Font(std::string &filePath) : filePath(filePath) {
        load();
    }

    ~Font() {

        for (auto &character: characters)
            if (character.second.textureID)
                glDeleteTextures(1, &character.second.textureID);
    }

    const Character &getCharacter(char c) const {
        return characters.at(c);
    }

    bool operator==(const Font& other) const {
        return filePath == other.filePath;
    }

private:

    void load() {

        if (!isLoaded) {
            loadLibs();
            isLoaded = true;
        }

        FT_Face face;
        if (FT_New_Face(ft, filePath.c_str(), 0, &face)) {
            AV_CORE_WARN("ERROR::FREETYPE: Failed to loadLibs font");
        }

        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                AV_CORE_WARN("ERROR::FREETYPE: Failed to loadLibs Glyph for character: {0}", c);
                continue;
            }

            uint32_t texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                    texture,
                    {face->glyph->bitmap.width, face->glyph->bitmap.rows},
                    {face->glyph->bitmap_left, face->glyph->bitmap_top},
                    static_cast<uint32_t>(face->glyph->advance.x)
            };


            characters[c] = character;
        }

        FT_Done_Face(face);
    }

    static void loadLibs() {

        if (FT_Init_FreeType(&ft)) {
            AV_CORE_WARN("ERROR::FREETYPE: Could not init FreeType Library");
            return;
        }
    }


    uint32_t workingSize;
    std::string filePath;
    std::map<char, Character> characters;

    static FT_Library ft;
    inline static bool isLoaded = false;
};
