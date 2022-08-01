// TYPE=VERTEX
#version 330 core
layout (location = 0) in vec4 aColor;
layout (location = 1) in vec3 aPos;
uniform mat4 uProjection;
uniform mat4 uTransform;
uniform mat4 uView;
void main()
{
    gl_Position = uProjection * uTransform * uView * vec4(aPos, 1.0);
}

// TYPE=FRAGMENT
#version 330 core
layout(location = 0) out vec4 FragColor;
uniform sampler2D uTexture;
in vec2 TexCoords;
void main()
{
    FragColor = vec4(1.0);
}