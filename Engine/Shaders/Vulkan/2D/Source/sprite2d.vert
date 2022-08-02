#version 450 core

layout (location = 0) in vec2 aPos;

layout(binding = 0) uniform Uniforms
{
    mat4 view;
    mat4 projection;
    mat4 model;
    float ar;
    bool keepAR;
} uniforms;

layout(location = 0) out vec2 TexCoords;

void main()
{
    vec2 scale = uniforms.keepAR ? vec2(uniforms.ar > 1 ? 1 / uniforms.ar : 1, uniforms.ar < 1 ? uniforms.ar : 1) : vec2(1.0);
    TexCoords = aPos.xy;
    gl_Position = uniforms.projection * uniforms.view * uniforms.model * vec4(aPos.xy * scale, 0.0, 1.0);
}