#version 330 core

in vec2 position;
// 裁剪空间坐标
out vec4 clipSpace;
out vec2 textureCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

// 平铺值
const float tiling = 6.0;

void main(void) {
	clipSpace = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
	gl_Position = clipSpace;
	// 纹理坐标 * 平铺值  进行平铺
	textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5) * tiling;
}