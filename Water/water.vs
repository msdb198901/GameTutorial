#version 330 core

in vec2 position;
// 裁剪空间坐标
out vec4 clipSpace;
out vec2 textureCoords;
// 顶点执行摄像机
out vec3 toCameraVector;
// 光源指向水体的向量
out vec3 fromLightVector;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

// 平铺值
const float tiling = 6.0;

void main(void) {
	vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;
	// 纹理坐标 * 平铺值  进行平铺
	textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5) * tiling;

	toCameraVector = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;
}