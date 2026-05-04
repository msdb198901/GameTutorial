#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;

out vec2 fragment_textureCoords;

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float numberOfRows;
uniform vec2 offset;

void main()
{
    vec4 worldPos = transformationMatrix * vec4(position, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
    fragment_textureCoords = (textureCoords / numberOfRows) + offset;
}