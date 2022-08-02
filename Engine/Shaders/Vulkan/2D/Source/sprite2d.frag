#version 450 core

layout(location = 0) out vec4 FragColor;

layout(binding = 1) uniform sampler2D uTexture;

layout(location = 0) in vec2 TexCoords;

void main()
{
    FragColor = texture(uTexture, TexCoords);
    if(FragColor.a < 0.35)
        discard;
    FragColor = pow(FragColor, vec4(1.0/2.2));
}