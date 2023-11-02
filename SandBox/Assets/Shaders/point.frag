#version 330 core
layout (location = 0) out vec4 oFragColor;
layout (location = 1) out int oEntityId;

in vec4 Color;
flat in float EntityId;

void main()
{
    oFragColor = Color;
    oEntityId = int(EntityId);
}