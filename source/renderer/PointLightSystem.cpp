#include "PointLightSystem.hpp"

#include "Scene.hpp"

#include "ShaderLoader.hpp"

#include "Mesh.hpp"
#include "PointLight.hpp"
#include "Transform.hpp"

namespace Kuma3D {

/******************************************************************************/
void PointLightSystem::Initialize(Scene& aScene)
{
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Mesh>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void PointLightSystem::Operate(Scene& aScene, double aTime)
{
  std::stringstream ss;

  // Retrieve all point lights in the scene.
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<PointLight>()] = true;
  signature[aScene.GetComponentIndex<Transform>()] = true;
  auto pointLights = aScene.GetEntitiesWithSignature(signature);
  for(int i = 0; i < pointLights.size(); ++i)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(pointLights[i]);
    auto& light = aScene.GetComponentForEntity<PointLight>(pointLights[i]);

    // For each point light, set the appropriate uniforms on each mesh.
    for(const auto& entity : GetEntities())
    {
      auto& mesh = aScene.GetComponentForEntity<Mesh>(entity);
      for(const auto& shader : mesh.mShaders)
      {
        ss << "pointLights[" << i << "].mPosition";
        ShaderLoader::SetVec3(shader, ss.str(), transform.mPosition);
        ss.str("");

        ss << "pointLights[" << i << "].mConstant";
        ShaderLoader::SetFloat(shader, ss.str(), light.mConstant);
        ss.str("");

        ss << "pointLights[" << i << "].mLinear";
        ShaderLoader::SetFloat(shader, ss.str(), light.mLinear);
        ss.str("");

        ss << "pointLights[" << i << "].mQuadratic";
        ShaderLoader::SetFloat(shader, ss.str(), light.mQuadratic);
        ss.str("");

        ss << "pointLights[" << i << "].mAmbientIntensity";
        ShaderLoader::SetFloat(shader, ss.str(), light.mAmbientIntensity);
        ss.str("");

        ss << "pointLights[" << i << "].mDiffuseIntensity";
        ShaderLoader::SetFloat(shader, ss.str(), light.mDiffuseIntensity);
        ss.str("");

        ss << "pointLights[" << i << "].mSpecularIntensity";
        ShaderLoader::SetFloat(shader, ss.str(), light.mSpecularIntensity);
        ss.str("");
      }
    }
  }
}

} // namespace Kuma3D
