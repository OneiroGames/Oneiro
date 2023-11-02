#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aEntityId;

uniform mat4 uProjection;
uniform mat4 uView;

out vec4 Color;
flat out float EntityId;

void main()
{
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
    Color = aColor;
    EntityId = aEntityId;
}  