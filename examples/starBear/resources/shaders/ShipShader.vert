#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec3 color;
out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
  color = aColor;
  texCoords = aTexCoords;

  vec2 lowRes = vec2(160, 80);
  vec4 pos = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0);
  pos.xyz = pos.xyz / pos.w;
  pos.xy = floor(lowRes * pos.xy) / lowRes;
  pos.xyz *= pos.w;

  gl_Position = pos;
}
