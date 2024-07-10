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
    vec2 fixed_resolution = vec2(800.0, 600.0);
    vec2 st = gl_FragCoord.xy / fixed_resolution;
    st.x *= fixed_resolution.x / fixed_resolution.y;

    vec3 color = vec3(0.);
    color = vec3(st.x, st.y, abs(sin(uTime)));

    gl_FragColor = vec4(color, 1.0);
}
