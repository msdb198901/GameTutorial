#version 330 core

in vec3 textureCoords;
out vec4 out_Color;

uniform samplerCube cubeMap;
uniform samplerCube cubeMap2;
// 混合因子 控制 两种天空盒纹理的混合比例
uniform float blendFactor;
// 雾效的颜色
uniform vec3 fogColor;

// 天空盒下限 下限天空盒为雾霾色 
// 介于两者之间则为雾霾色与天空盒纹理的混合效果 线性渐变
const float lowerLimit = 0.0;
// 天空盒上限 上限天空盒为天空盒纹理
const float upperLimit = 30.0;

void main(void){
    // 天空盒纹理的采样
    vec4 texture1 = texture(cubeMap, textureCoords);
    vec4 texture2 = texture(cubeMap2, textureCoords);
    vec4 finalColor = mix(texture1, texture2, blendFactor);
    // 代表天空盒片段的可见度
    float factor = (textureCoords.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0, 1.0);
    out_Color = mix(vec4(fogColor, 1.0), finalColor, factor);
}