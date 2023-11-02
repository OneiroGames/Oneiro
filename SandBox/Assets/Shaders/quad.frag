#version 330 core
layout (location = 0) out vec4 oFragColor;
layout (location = 1) out int oEntityId;

in vec4 Color;
in vec2 TexCoords;
flat in float TexIndex;
flat in float EntityId;

uniform sampler2D uTextures[32];

void main()
{
    vec4 color = vec4(1.0);
    switch (int(TexIndex))
    {
        case  0: color = Color; break;
		case  1: color = pow(texture(uTextures[ 1], TexCoords), vec4(1.0/2.2)); break;
		case  2: color = pow(texture(uTextures[ 2], TexCoords), vec4(1.0/2.2)); break;
		case  3: color = pow(texture(uTextures[ 3], TexCoords), vec4(1.0/2.2)); break;
		case  4: color = pow(texture(uTextures[ 4], TexCoords), vec4(1.0/2.2)); break;
		case  5: color = pow(texture(uTextures[ 5], TexCoords), vec4(1.0/2.2)); break;
		case  6: color = pow(texture(uTextures[ 6], TexCoords), vec4(1.0/2.2)); break;
		case  7: color = pow(texture(uTextures[ 7], TexCoords), vec4(1.0/2.2)); break;
		case  8: color = pow(texture(uTextures[ 8], TexCoords), vec4(1.0/2.2)); break;
		case  9: color = pow(texture(uTextures[ 9], TexCoords), vec4(1.0/2.2)); break;
		case 10: color = pow(texture(uTextures[10], TexCoords), vec4(1.0/2.2)); break;
		case 11: color = pow(texture(uTextures[11], TexCoords), vec4(1.0/2.2)); break;
		case 12: color = pow(texture(uTextures[12], TexCoords), vec4(1.0/2.2)); break;
		case 13: color = pow(texture(uTextures[13], TexCoords), vec4(1.0/2.2)); break;
		case 14: color = pow(texture(uTextures[14], TexCoords), vec4(1.0/2.2)); break;
		case 15: color = pow(texture(uTextures[15], TexCoords), vec4(1.0/2.2)); break;
		case 16: color = pow(texture(uTextures[16], TexCoords), vec4(1.0/2.2)); break;
		case 17: color = pow(texture(uTextures[17], TexCoords), vec4(1.0/2.2)); break;
		case 18: color = pow(texture(uTextures[18], TexCoords), vec4(1.0/2.2)); break;
		case 19: color = pow(texture(uTextures[19], TexCoords), vec4(1.0/2.2)); break;
		case 20: color = pow(texture(uTextures[20], TexCoords), vec4(1.0/2.2)); break;
		case 21: color = pow(texture(uTextures[21], TexCoords), vec4(1.0/2.2)); break;
		case 22: color = pow(texture(uTextures[22], TexCoords), vec4(1.0/2.2)); break;
		case 23: color = pow(texture(uTextures[23], TexCoords), vec4(1.0/2.2)); break;
		case 24: color = pow(texture(uTextures[24], TexCoords), vec4(1.0/2.2)); break;
		case 25: color = pow(texture(uTextures[25], TexCoords), vec4(1.0/2.2)); break;
		case 26: color = pow(texture(uTextures[26], TexCoords), vec4(1.0/2.2)); break;
		case 27: color = pow(texture(uTextures[27], TexCoords), vec4(1.0/2.2)); break;
		case 28: color = pow(texture(uTextures[28], TexCoords), vec4(1.0/2.2)); break;
		case 29: color = pow(texture(uTextures[29], TexCoords), vec4(1.0/2.2)); break;
		case 30: color = pow(texture(uTextures[30], TexCoords), vec4(1.0/2.2)); break;
		case 31: color = pow(texture(uTextures[31], TexCoords), vec4(1.0/2.2)); break;
    }

	oEntityId = int(EntityId);

	if (color.a < 0.25)
		discard;

    oFragColor = color;
}