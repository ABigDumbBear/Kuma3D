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
  vec3 ambient = vec3(0.1, 0.1, 0.1);

  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 lightDir = normalize(lightPos - fragPos);
  vec3 norm = normalize(normal);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 viewPos = vec3(0.0, 0.0, 0.0);
  float specularStrength = 1.0;
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * vec3(texture(texSampler, texCoords));
  fragColor = vec4(result, 1.0);
}
