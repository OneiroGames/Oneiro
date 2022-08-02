// TYPE=VERTEX
#version 330 core
layout(location = 0) in vec4 aColor;
layout(location = 1) in vec4 aWorldPosition;
layout(location = 2) in vec4 aLocalPosition;
layout(location = 3) in float aThickness;
layout(location = 4) in float aFade;
uniform mat4 uProjection;
uniform vec2 uResolution;
out vec4 Color;
out vec2 UV;
out float Thickness;
out float Fade;
void main()
{
    gl_Position = uProjection * vec4(aWorldPosition.xyz, 1.0);
    Color = aColor;
    UV = aLocalPosition.xy * 2.0 - 1.0; UV.x *= (uResolution.x / uResolution.y);
    Thickness = aThickness;
    Fade = aFade;
}

// TYPE=FRAGMENT
#version 330 core
out vec4 FragColor;
in vec4 Color;
in vec2 UV;
in float Thickness;
in float Fade;
void main()
{
    float distance = 1.0 - length(UV);
    float circle = smoothstep(0.0, Fade, distance);
    circle *= smoothstep(Thickness + Fade, Thickness, distance);

    if (circle == 0.0) discard;

    FragColor = Color;
    FragColor.a *= circle;
}