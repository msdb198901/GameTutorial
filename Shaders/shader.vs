#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

out vec2 fragment_textureCoords;
out vec3 surfaceNormal;
// 向量数组 多光源 4个光源 光源越多游戏运行越慢
out vec3 toLightVector[4];
out vec3 toCameraVector; 
// 每个顶点计算可见度
out float visibility;

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPosition[4];

uniform float useFakeLighting;
uniform float numberOfRows;
uniform vec2  offset;
uniform vec4  clipPlane;

// 雾的密度值
const float density = 0.0035;
// 雾的梯度值
const float gradient = 5.0;

void main()
{
   vec4 world_position = transformationMatrix * vec4(position, 1.0);

   gl_ClipDistance[0] = dot(world_position, clipPlane);

   // 顶点相对于摄像机距离
   vec4 positionRelativeToCamera = viewMatrix * world_position;
   gl_Position = projectionMatrix * positionRelativeToCamera;
   fragment_textureCoords = (textureCoords/numberOfRows) + offset;

   vec3 actualNormal = normal;
   if (useFakeLighting > 0.5)
   {
	  actualNormal = vec3(0, 1, 0);
   }

   surfaceNormal = (transformationMatrix * vec4(actualNormal, 0.0)).xyz;
   
   for (int i = 0; i < 4; i++)
   {
	   toLightVector[i] = lightPosition[i] - world_position.xyz;
   }
   toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - world_position.xyz;

   float distance = length(positionRelativeToCamera.xyz);
   visibility = exp(-pow((distance*density), gradient));
   // 限制值在0-1之间
   visibility = clamp(visibility, 0, 1);
}
