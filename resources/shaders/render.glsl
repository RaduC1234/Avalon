#type vertex
#version 430 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec2 aTexCoords;
layout (location=3) in float aTexId;
layout (location=4) in float aShapeType;

uniform mat4 uWorldProjection;
uniform mat4 uView;

out vec4 fColor;
out vec2 fTexCoords;
out float fTexId;
out float fShapeType;

void main()
{
    fColor = aColor;
    fTexCoords = aTexCoords;
    fTexId = aTexId;
    fShapeType = aShapeType;

    gl_Position = uWorldProjection * uView * vec4(aPos, 1.0);
}

#type fragment
#version 430 core

in vec4 fColor;
in vec2 fTexCoords;
in float fTexId;
in float fShapeType;

uniform sampler2D uTextures[16];

out vec4 color;

void main() {

    if (fTexId > 0) {
        int id = int(fTexId);
        color = fColor * texture(uTextures[id], fTexCoords);
    } else {
        color = fColor;
    }

}
