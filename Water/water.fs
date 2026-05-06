#version 330 core

in vec4 clipSpace;
out vec4 out_Color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main(void) 
{
    vec2 ndc = clipSpace.xy / clipSpace.w;
    // 屏幕空间坐标
    vec2 textureCoords = ndc * 0.5 + 0.5;
    // 反射和折射纹理坐标
    vec2 refractCoords = vec2(textureCoords.x, textureCoords.y);
    vec2 reflectCoords = vec2(textureCoords.x, 1.0-textureCoords.y);

    vec4 reflectionColor = texture(reflectionTexture, reflectCoords);
    vec4 refractionColor = texture(refractionTexture, refractCoords);
    out_Color = mix(refractionColor, reflectionColor, 0.5);
}