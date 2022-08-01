// TYPE=VERTEX
#version 330 core
layout (location = 0) in vec4 aColor;
layout (location = 1) in vec4 aPos;
layout (location = 2) in float aSize;
uniform mat4 uProjection;
out vec4 Color;
void main()
{
    gl_PointSize = aSize;
    gl_Position = uProjection * vec4(aPos.xyz, 1.0);;
    Color = aColor;
}

// TYPE=FRAGMENT
#version 330 core
out vec4 FragColor;
in vec4 Color;
void main()
{
    FragColor = Color;
}