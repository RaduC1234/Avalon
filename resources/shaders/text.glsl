#type vertex
#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 pos, vec2 tex>

uniform mat4 uProjection;

out vec2 fTexCoords;

void main()
{
    gl_Position = uProjection * vec4(aVertex.xy, 0.0, 1.0);
    fTexCoords = aVertex.zw;
}

#type fragment
#version 330 core

in vec2 fTexCoords;

out vec4 color;

uniform sampler2D uText;
uniform vec3 uTextColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uText, fTexCoords).r);
    color = vec4(uTextColor, 1.0) * sampled;
}
