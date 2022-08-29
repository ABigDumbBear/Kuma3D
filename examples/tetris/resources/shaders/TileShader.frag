#version 330 core

in vec3 color;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texSampler;

void main()
{
  fragColor = mix(texture(texSampler, texCoords), vec4(color, 1.0), 0.2);
}
