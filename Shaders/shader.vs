#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

out vec2 fragment_textureCoords;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector; 

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPosition;

uniform float useFakeLighting;

void main()
{
   vec4 world_position = transformationMatrix * vec4(position, 1.0);
   gl_Position = projectionMatrix * viewMatrix * world_position;
   fragment_textureCoords = textureCoords;

   vec3 actualNormal = normal;
   if (useFakeLighting > 0.5)
   {
	  actualNormal = vec3(0, 1, 0);
   }

   surfaceNormal = (transformationMatrix * vec4(actualNormal, 0.0)).xyz;
   toLightVector = lightPosition - world_position.xyz;
   toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - world_position.xyz;
}
