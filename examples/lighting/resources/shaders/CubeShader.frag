#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texSampler;
uniform vec3 lightPos;

struct Material
{
  vec3 mAmbient;
  vec3 mDiffuse;
  vec3 mSpecular;
  float mShininess;
};

uniform Material material;

void main()
{
  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 lightDir = normalize(lightPos - fragPos);

  float diff = max(dot(normalize(normal), lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  vec3 ambient = vec3(0.1, 0.1, 0.1);

  fragColor = (vec4(diffuse, 1.0) + vec4(ambient, 1.0)) * texture(texSampler, texCoords);
}
