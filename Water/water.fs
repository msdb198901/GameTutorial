//#version 330 core

//in vec4 clipSpace;
//in vec2 textureCoords;
//in vec3 toCameraVector;
//in vec3 fromLightVector;

//out vec4 out_Color;

//uniform sampler2D reflectionTexture;
//uniform sampler2D refractionTexture;
//uniform sampler2D dudvMap;
//uniform sampler2D normalMap;
//uniform vec3 lightColor;

//uniform float moveFactor;

//const float waveStrength = 0.02;
//const float shineDamper = 20.0;
//const float reflectivity = 0.6;

//void main(void) 
//{
//    vec2 ndc = clipSpace.xy / clipSpace.w;
//    // 屏幕空间坐标
//    vec2 textureCoords = ndc * 0.5 + 0.5;
//    // 反射和折射纹理坐标
//    vec2 refractCoords = vec2(textureCoords.x, textureCoords.y);
//    vec2 reflectCoords = vec2(textureCoords.x, -textureCoords.y);

//    //// 扰动纹理坐标
//    //vec2 distord1 = (texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 2.0 - 1.0) * waveStrength;
//    //vec2 distord2 = (texture(dudvMap, vec2(-textureCoords.x + moveFactor, textureCoords.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;
//    //vec2 distord = distord1 + distord2;

//    // 更好实现扰动纹理坐标
//    vec2 distord = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
//    distord = textureCoords + vec2(distord.x, distord.y + moveFactor);
//    distord = (texture(dudvMap, distord).rg * 2.0 - 1.0) * waveStrength;

//    refractCoords += distord;
//    refractCoords = clamp(refractCoords, 0.001, 0.999);

//    reflectCoords += distord;
//    reflectCoords.x = clamp(reflectCoords.x, 0.001, 0.999);
//    reflectCoords.y = clamp(reflectCoords.y, -0.999, -0.001);

//    vec4 reflectionColor = texture(reflectionTexture, reflectCoords);
//    vec4 refractionColor = texture(refractionTexture, refractCoords);

//    vec3 viewVector = normalize(toCameraVector);
//    // 视角向量与水面法线向量的点乘，得到折射率
//    float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
//    refractiveFactor = pow(abs(refractiveFactor), 0.5);

//    // 水面法线
//    vec4 normalMapColor = texture(normalMap, distord);
//    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g* 2.0 - 1.0);
//    normal = normalize(normal);

//    vec3 reflectLight = reflect(normalize(fromLightVector), normal);
//    float specular = max(dot(viewVector, reflectLight), 0.0);
//    specular = pow(specular, shineDamper);
//    vec3 specularHighlights = lightColor * specular * reflectivity;
    
//    out_Color = mix(reflectionColor, refractionColor, refractiveFactor);
//    out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0);
//}


#version 330 core

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;

out vec4 out_Color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform vec3 lightColor;
uniform float moveFactor;

const float waveStrength = 0.04;
const float shineDamper = 20.0;
const float reflectivity = 0.6;

void main(void) 
{
    // 屏幕空间坐标
    vec2 ndc = clipSpace.xy / clipSpace.w;
    vec2 screenCoords = ndc * 0.5 + 0.5;
    
    // 折射坐标（直接使用屏幕坐标）
    vec2 refractCoords = screenCoords;
    // 反射坐标（Y方向翻转）
    vec2 reflectCoords = vec2(screenCoords.x, 1.0 - screenCoords.y);

    // 深度测试（可选，用于水下效果）
    // 与主渲染器设定保持一致 最好uniform float near, far; 定义在主渲染器中
    float near = 0.1;
    float far = 1000.0;
    float depth = texture(depthMap, refractCoords).r;
    float floorDistance = 2.0 * near* far/(far+near - (2.0*depth - 1.0)*(far - near));
    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near* far/(far+near - (2.0*depth - 1.0)*(far - near));
    float waterDepth = floorDistance - waterDistance;
    
    // ----- 扰动计算（经典两次叠加，产生流动感）-----
    vec2 distortedUV1 = textureCoords + vec2(moveFactor, 0.0);
    vec2 distortedUV2 = textureCoords + vec2(0.0, moveFactor);
    
    vec2 distord1 = texture(dudvMap, distortedUV1).rg * 2.0 - 1.0;
    vec2 distord2 = texture(dudvMap, distortedUV2).rg * 2.0 - 1.0;
    vec2 distord = (distord1 + distord2) * waveStrength * clamp(waterDepth/20, 0.0, 1.0);
    
    // 对反射和折射纹理坐标分别添加扰动
    reflectCoords += distord;
    refractCoords += distord;
    
    // 钳位避免边缘采样黑边
    reflectCoords = clamp(reflectCoords, 0.001, 0.999);
    refractCoords = clamp(refractCoords, 0.001, 0.999);
    
    vec4 reflectionColor = texture(reflectionTexture, reflectCoords);
    vec4 refractionColor = texture(refractionTexture, refractCoords);

    // ----- 法线贴图（根据扰动后的UV采样）-----
    vec2 normalUV = textureCoords + distord * 2.0;  // 让法线扰动幅度稍大
    vec4 normalMapColor = texture(normalMap, normalUV);
    // 假设法线贴图存储为 R=X, G=Z, B=Y  (常见于某些引擎，需确认)
    vec3 normal;
    normal.x = normalMapColor.r * 2.0 - 1.0;
    normal.z = normalMapColor.g * 2.0 - 1.0;
    normal.y = normalMapColor.b * 2.0 - 1.0;
    normal = normalize(normal);
    
    // ----- 菲涅尔效应（视角与水面的夹角）-----
    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(1.0 - refractiveFactor, 0.5); // 修正：角度越小反射越强
   
    
    // ----- 高光反射 -----
    vec3 lightDir = normalize(fromLightVector);
    vec3 reflectDir = reflect(lightDir, normal);
    float specular = max(dot(viewVector, reflectDir), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = lightColor * specular * reflectivity * clamp(waterDepth/5, 0.0, 1.0);
    
    // ----- 混合颜色 -----
    vec4 waterColor = mix(reflectionColor, refractionColor, refractiveFactor);
    // 加入浅水底色并加上高光
    waterColor = mix(waterColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
    out_Color = waterColor + vec4(specularHighlights, 0.0);

    // 加入水下深度效果（可选）
    out_Color.a = clamp(waterDepth/5, 0.0, 1.0);
}