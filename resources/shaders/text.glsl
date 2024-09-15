#type vertex
#version 430 core
layout (location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in uint aGlyphID;

struct Glyph {
    vec2 size;    // Glyph size
    vec2 bearing; // Offset from baseline to top/left of the glyph
    vec2 texCoords; // Offset for texture coordinates in the glyph map
    float advance; // Horizontal advance to the next glyph
};

uniform mat4 uProjection;

// SSBO for glyph data, each glyph has its metadata stored in the SSBO
layout(std430, binding = 0) buffer GlyphData {
    Glyph glyphs[];  // Array of glyphs in the SSBO
};

out vec2 fTexCoords;

void main() {
    Glyph glyph = glyphs[int(aGlyphID)];

    // Apply gpyph size and bearing to position the glyph correctly
    vec3 glyphPos = aPosition * vec3(glyph.size, 1.0) + vec3(glyph.bearing, 0.0);
    gl_Position = uProjection * vec4(glyphPos, 1.0);

   // fTexCoords = mix(glyph.texCoords.xy, glyph.texCoords.zw, aTexCoords);
}

#type fragment
#version 430 core

in vec2 fTexCoords;

out vec4 color;

uniform sampler2D uTextAtlas;
uniform vec3 uTextColor;

void main() {
    float alpha = texture(uTextAtlas, fTexCoords).r;
    color = vec4(uTextColor, alpha);
}
