#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  fragPos = vec3(modelMatrix * vec4(aPosition, 1.0));
  normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
  texCoords = aTexCoords;

  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0);
}
