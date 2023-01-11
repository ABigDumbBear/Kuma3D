#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texSampler;
uniform vec3 lightPos;

struct PointLight
{
  vec3 mPosition;

  float mConstant;
  float mLinear;
  float mQuadratic;

  float mAmbientIntensity;
  float mDiffuseIntensity;
  float mSpecularIntensity;
};

uniform PointLight pointLights[5];

struct Material
{
  vec3 mAmbient;
  vec3 mDiffuse;
  vec3 mSpecular;
  float mShininess;
};

uniform Material material;

vec3 CalcPointLight(PointLight aLight, vec3 aNormal, vec3 aFragPos, vec3 aViewDir)
{
}

void main()
{
  vec3 result = (ambient + diffuse + specular) * vec3(texture(texSampler, texCoords));
  fragColor = vec4(result, 1.0);
}
