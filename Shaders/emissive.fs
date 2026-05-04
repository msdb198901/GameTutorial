#version 330 core
in vec2 fragment_textureCoords;
out vec4 outColor;

uniform sampler2D textureSampler;
uniform vec3 emissiveColor = vec3(1.0, 0.85, 0.55);   // 暖黄色自发光

void main()
{
    vec4 texColor = texture(textureSampler, fragment_textureCoords);
    if (texColor.a < 0.5) discard;
    
    // 自发光：保留纹理颜色，但增强亮度并叠加 emissiveColor
    vec3 finalColor = texColor.rgb * emissiveColor * 1.8;
    outColor = vec4(finalColor, texColor.a);
}