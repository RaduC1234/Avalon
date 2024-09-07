#type vertex
#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 pos, vec2 tex>
out vec2 fTexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aVertex.xy, 0.0, 1.0);
    fTexCoords = aVertex.zw;
}

#type fragment
#version 330 core

in vec2 fTexCoords;

out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, fTexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
