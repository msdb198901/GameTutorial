#version 330 core

in vec2 fragment_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 outColor;

uniform sampler2D textureSampler;
uniform vec3 lightColor[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;
uniform vec3 attenuation[4];


void main()
{
    vec3 unitCameraVector = normalize(toCameraVector);
    vec3 unitNormal = normalize(surfaceNormal);

    vec3 totalDiffuse = vec3(0.0);
    vec3 totalFinalSpecular = vec3(0.0);
    for (int i = 0; i < 4; ++i)
    {
        float distance = length(toLightVector[i]);
        // 计算衰减因子
        float attenuationFactor = attenuation[i].x + (attenuation[i].y * distance) + (attenuation[i].z * (distance * distance));

        vec3 unitLightVector = normalize(toLightVector[i]);
        float brightness = dot(unitNormal, unitLightVector);
        brightness = max(0.0, brightness);

        vec3 lightDirection = -unitLightVector;
        vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
        float specularFactor = dot(reflectedLightDirection, unitCameraVector);
        specularFactor = max(0.0, specularFactor);
        float dampedFactor = pow(specularFactor, shineDamper);

        vec3 diffuse = brightness * lightColor[i] / attenuationFactor;
        vec3 finalSpecular = dampedFactor * reflectivity * lightColor[i] / attenuationFactor;

        totalDiffuse += diffuse;
        totalFinalSpecular += totalFinalSpecular;
    }
    // 防止光照过暗 漫反射至少为0.2
    totalDiffuse = max(totalDiffuse, 0.2);
    
    vec4 textureColor = texture(textureSampler, fragment_textureCoords);
    if (textureColor.a < 0.5)
    {
        discard;
    }
    outColor = vec4(totalDiffuse, 1.0) * textureColor + vec4(totalFinalSpecular, 1.0);
    // 进行天空颜色和物体颜色混合
    outColor = mix(vec4(skyColor, 1.0), outColor, visibility);
}
