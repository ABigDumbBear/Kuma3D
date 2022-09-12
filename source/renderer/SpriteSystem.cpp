#include "SpriteSystem.hpp"

#include "Scene.hpp"

#include "TextureLoader.hpp"

#include "Mesh.hpp"
#include "Sprite.hpp"

namespace Kuma3D {

/******************************************************************************/
void SpriteSystem::Initialize(Scene& aScene)
{
  if(!aScene.IsComponentTypeRegistered<Mesh>())
  {
    aScene.RegisterComponentType<Mesh>();
  }

  if(!aScene.IsComponentTypeRegistered<Sprite>())
  {
    aScene.RegisterComponentType<Sprite>();
  }

  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Mesh>()] = true;
  signature[aScene.GetComponentIndex<Sprite>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void SpriteSystem::Operate(Scene& aScene, double aTime)
{
  // For each newly added Entity, update the Mesh immediately.
  for(const auto& entity : mNewEntities)
  {
    auto& sprite = aScene.GetComponentForEntity<Sprite>(entity);
    auto& mesh = aScene.GetComponentForEntity<Mesh>(entity);
    UpdateMeshToDisplaySprite(mesh, sprite);

    mEntityTimeMap[entity] = aTime;
  }
  mNewEntities.clear();

  for(const auto& entity : GetEntities())
  {
    auto& sprite = aScene.GetComponentForEntity<Sprite>(entity);
    auto dt = aTime - mEntityTimeMap[entity];

    if(dt > (1.0 / sprite.mAnimationSpeed))
    {
      // Move the next frame of the animation (or back to the first).
      auto& animation = sprite.mAnimations[sprite.mCurrentAnimation];
      auto& frame = animation.mCurrentFrame;
      frame == animation.mFrames.size() - 1 ? frame = 0 : ++frame;

      // Retrieve the Mesh and update its vertices according to the next
      // frame of the animation.
      if(!animation.mFrames.empty())
      {
        auto& mesh = aScene.GetComponentForEntity<Mesh>(entity);
        UpdateMeshToDisplaySprite(mesh, sprite);

        // Update the time map for this Entity.
        mEntityTimeMap[entity] = aTime;
      }
    }
  }
}

/******************************************************************************/
void SpriteSystem::HandleEntityBecameEligible(const Entity& aEntity)
{
  mNewEntities.emplace_back(aEntity);
  mEntityTimeMap.emplace(aEntity, 0);
}

/******************************************************************************/
void SpriteSystem::HandleEntityBecameIneligible(const Entity& aEntity)
{
  auto foundEntity = std::find(mNewEntities.begin(), mNewEntities.end(), aEntity);
  if(foundEntity != mNewEntities.end())
  {
    mNewEntities.erase(foundEntity);
  }

  mEntityTimeMap.erase(aEntity);
}

/******************************************************************************/
void SpriteSystem::UpdateMeshToDisplaySprite(Mesh& aMesh, const Sprite& aSprite)
{
  // Clear out any previous data.
  aMesh.mVertices.clear();
  aMesh.mTextures.clear();
  aMesh.mIndices.clear();

  const auto& animation = aSprite.mAnimations.at(aSprite.mCurrentAnimation);
  const auto& textureClip = animation.mFrames[animation.mCurrentFrame];

  auto dimensions = TextureLoader::GetTextureDimensions(aSprite.mSpritesheetTextureID);
  float textureWidth = dimensions.first;
  float textureHeight = dimensions.second;
  float meshWidth = textureClip.mRight - textureClip.mLeft;
  float meshHeight = textureClip.mTop - textureClip.mBottom;

  // Create a 2D quad to display the sprite with.
  MeshVertex vertex;

  vertex.mPosition = Vec3(0, 0, 0);
  vertex.mTexCoords[0] = textureClip.mLeft / textureWidth;
  vertex.mTexCoords[1] = textureClip.mBottom / textureHeight;
  aMesh.mVertices.emplace_back(vertex);

  vertex.mPosition = Vec3(meshWidth, 0, 0);
  vertex.mTexCoords[0] = textureClip.mRight / textureWidth;
  vertex.mTexCoords[1] = textureClip.mBottom / textureHeight;
  aMesh.mVertices.emplace_back(vertex);

  vertex.mPosition = Vec3(meshWidth, meshHeight, 0);
  vertex.mTexCoords[0] = textureClip.mRight / textureWidth;
  vertex.mTexCoords[1] = textureClip.mTop / textureHeight;
  aMesh.mVertices.emplace_back(vertex);

  vertex.mPosition = Vec3(0, meshHeight, 0);
  vertex.mTexCoords[0] = textureClip.mLeft / textureWidth;
  vertex.mTexCoords[1] = textureClip.mTop / textureHeight;
  aMesh.mVertices.emplace_back(vertex);

  // Add the vertex indices.
  aMesh.mIndices.emplace_back(0);
  aMesh.mIndices.emplace_back(1);
  aMesh.mIndices.emplace_back(3);
  aMesh.mIndices.emplace_back(3);
  aMesh.mIndices.emplace_back(1);
  aMesh.mIndices.emplace_back(2);

  // Add the texture ID.
  aMesh.mTextures.emplace_back(aSprite.mSpritesheetTextureID);

  aMesh.mHasTransparency = true;
  aMesh.mDirty = true;
}

} // namespace Kuma3D
