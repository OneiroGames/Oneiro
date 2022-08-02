// TYPE=VERTEX
#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in int aTexIndex;
layout (location = 4) in float aAlpha;
layout (location = 5) in float aAr;
layout (location = 6) in int aKeepAr;
uniform mat4 uProjection;
out vec4 Color;
out vec2 TexCoords;
flat out int TexIndex;
out float Alpha;
void main()
{
    vec2 scale = (aKeepAr == 1) ? vec2(aAr > 1 ? 1 / aAr : 1, aAr < 1 ? aAr : 1) : vec2(1.0);
    gl_Position = uProjection * vec4(aPos.xy * scale, 0.0, 1.0);
    Color = aColor;
    TexCoords = aTexCoords;
    TexIndex = aTexIndex;
    Alpha = aAlpha;
}

// TYPE=FRAGMENT
#version 330 core
out vec4 FragColor;
uniform sampler2D uTextures[32];
in vec4 Color;
in vec2 TexCoords;
flat in int TexIndex;
in float Alpha;
void main()
{
    vec4 Texture;

    switch(TexIndex)
    {
    case  0: Texture = texture(uTextures[0], TexCoords); break;
    case  1: Texture = texture(uTextures[1], TexCoords); break;
    case  2: Texture = texture(uTextures[2], TexCoords); break;
    case  3: Texture = texture(uTextures[3], TexCoords); break;
    case  4: Texture = texture(uTextures[4], TexCoords); break;
    case  5: Texture = texture(uTextures[5], TexCoords); break;
    case  6: Texture = texture(uTextures[6], TexCoords); break;
    case  7: Texture = texture(uTextures[7], TexCoords); break;
    case  8: Texture = texture(uTextures[8], TexCoords); break;
    case  9: Texture = texture(uTextures[9], TexCoords); break;
    case 10: Texture = texture(uTextures[10], TexCoords); break;
    case 11: Texture = texture(uTextures[11], TexCoords); break;
    case 12: Texture = texture(uTextures[12], TexCoords); break;
    case 13: Texture = texture(uTextures[13], TexCoords); break;
    case 14: Texture = texture(uTextures[14], TexCoords); break;
    case 15: Texture = texture(uTextures[15], TexCoords); break;
    case 16: Texture = texture(uTextures[16], TexCoords); break;
    case 17: Texture = texture(uTextures[17], TexCoords); break;
    case 18: Texture = texture(uTextures[18], TexCoords); break;
    case 19: Texture = texture(uTextures[19], TexCoords); break;
    case 20: Texture = texture(uTextures[20], TexCoords); break;
    case 21: Texture = texture(uTextures[21], TexCoords); break;
    case 22: Texture = texture(uTextures[22], TexCoords); break;
    case 23: Texture = texture(uTextures[23], TexCoords); break;
    case 24: Texture = texture(uTextures[24], TexCoords); break;
    case 25: Texture = texture(uTextures[25], TexCoords); break;
    case 26: Texture = texture(uTextures[26], TexCoords); break;
    case 27: Texture = texture(uTextures[27], TexCoords); break;
    case 28: Texture = texture(uTextures[28], TexCoords); break;
    case 29: Texture = texture(uTextures[29], TexCoords); break;
    case 30: Texture = texture(uTextures[30], TexCoords); break;
    case 31: Texture = texture(uTextures[31], TexCoords); break;
    default: Texture = Color; break;
    }

    if (TexIndex > 0)
        Texture = pow(Texture, vec4(1.0/2.2));

    FragColor = Texture;
}