#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    float glyphShape = texture(text, TexCoords).r;

    if (glyphShape < 0.5)
        discard;

    FragColor = vec4(1.0);
}