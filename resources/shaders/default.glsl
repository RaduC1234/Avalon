#type vertex
#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoords;

uniform mat4 uProjection;
uniform mat4 uView;

out vec4 fColor;
out vec2 fTexCoords;

void main()
{
    fColor = aColor;
    fTexCoords = aTexCoords;
    gl_Position =  uProjection * uView * vec4(aPos, 1.0);
}

#type fragment
#version 330 core

uniform float uTime;
uniform sampler2D TEX_SAMPLER;

in vec4 fColor;
in vec2 fTexCoords;

out vec4 color;

void main() {
    // Sample the texture at the given texture coordinates
    vec4 textureColor = texture(TEX_SAMPLER, fTexCoords);

    // Combine the texture color with the vertex color
    // You can use different blending techniques; here, we simply multiply them
    color = textureColor * fColor;
}
