#version 330 core
layout (location = 0) in vec4 aPos;
out vec2 TexCoords;

uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * vec4(aPos.xy, 0.0, 1.0);
    TexCoords = aPos.zw;
}