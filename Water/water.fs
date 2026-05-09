#version 330 core

in vec4 clipSpace;
in vec2 textureCoords;
out vec4 out_Color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform float moveFactor;

const float waveStrength = 0.02;

void main(void) 
{
    vec2 ndc = clipSpace.xy / clipSpace.w;
    // 屏幕空间坐标
    vec2 textureCoords = ndc * 0.5 + 0.5;
    // 反射和折射纹理坐标
    vec2 refractCoords = vec2(textureCoords.x, textureCoords.y);
    vec2 reflectCoords = vec2(textureCoords.x, 1.0-textureCoords.y);

    // 扰动纹理坐标
    vec2 distord1 = (texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distord2 = (texture(dudvMap, vec2(-textureCoords.x + moveFactor, textureCoords.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distord = (distord1 + distord2)/2;

    refractCoords += distord;
    refractCoords = clamp(refractCoords, 0.001, 0.999);

    reflectCoords += distord;
    reflectCoords = clamp(reflectCoords, 0.001, 0.999);

    vec4 reflectionColor = texture(reflectionTexture, reflectCoords);
    vec4 refractionColor = texture(refractionTexture, refractCoords);
    out_Color = mix(refractionColor, reflectionColor, 0.5);
    out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
}