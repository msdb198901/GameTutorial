#version 330 core

in vec2 fragment_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 outColor;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;
uniform sampler2D blendMap;

uniform vec3 lightColor[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;


void main()
{
    vec4 blendMapColor = texture(blendMap, fragment_textureCoords);
    // 计算强度 用于背景纹理的渲染 背景纹理在混合贴图为黑色时显示
    float totalWeight = blendMapColor.r + blendMapColor.g + blendMapColor.b;
    float backTextureAlpha = clamp(1.0 - totalWeight, 0.0, 1.0);
    // 计算平铺纹理坐标 用于纹理的平铺渲染 平铺纹理坐标的倍数越大，纹理越不明显
    vec2 tiledCoords = fragment_textureCoords * 40.0;
    // 计算背景纹理颜色和混合贴图纹理颜色
    vec4 backTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAlpha;
    // 计算三种纹理颜色和混合贴图颜色的乘积，用于纹理的渲染
    vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
    vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
    vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;
    // 计算三种纹理颜色和混合贴图颜色的总和，用于纹理的渲染
    vec4 totalColor = backTextureColor + rTextureColor + gTextureColor + bTextureColor;

    // 光照计算
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalFinalSpecular = vec3(0.0);
    for (int i = 0; i < 4; i++) {
        vec3 unitLightVector = normalize(toLightVector[i]);
        float brightness = dot(unitNormal, unitLightVector);
        brightness = max(0.2, brightness);
   
        vec3 unitCameraVector = normalize(toCameraVector);
        vec3 lightDirection = -unitLightVector;
        vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
        float specularFactor = dot(reflectedLightDirection, unitCameraVector);
        specularFactor = max(0.0, specularFactor);
        float dampedFactor = pow(specularFactor, shineDamper);

        vec3 diffuse = brightness * lightColor[i];
        vec3 finalSpecular = dampedFactor * reflectivity * lightColor[i];

        totalDiffuse += diffuse;
        totalFinalSpecular += finalSpecular;
    }
    totalDiffuse = max(totalDiffuse, 0.2);

    outColor = vec4(totalDiffuse, 1.0) * totalColor + vec4(totalFinalSpecular, 1.0);
    // 进行天空颜色和物体颜色混合
    outColor = mix(vec4(skyColor, 1.0), outColor, visibility);
}
