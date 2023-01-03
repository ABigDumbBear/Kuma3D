#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  texCoords = aTexCoords;

  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0);
}
