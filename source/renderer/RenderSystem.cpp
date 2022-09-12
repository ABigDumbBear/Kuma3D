#include "RenderSystem.hpp"

#include <algorithm>

#include <GL/glew.h>

#include "Scene.hpp"

#include "Mat4.hpp"
#include "MathUtil.hpp"
#include "Vec3.hpp"

#include "GameSignals.hpp"

#include "ShaderLoader.hpp"

#include "Camera.hpp"
#include "Transform.hpp"

namespace Kuma3D {

/******************************************************************************/
Mat4 CalculateModelMatrix(const Transform& aTransform)
{
  auto translationMatrix = Translate(aTransform.mPosition);
  auto scalarMatrix = Scale(aTransform.mScalar);
  auto rotationMatrix = Rotate(Vec3(1.0, 0.0, 0.0), aTransform.mRotation.x);
  rotationMatrix = rotationMatrix * Rotate(Vec3(0.0, 1.0, 0.0), aTransform.mRotation.y);
  rotationMatrix = rotationMatrix * Rotate(Vec3(0.0, 0.0, 1.0), aTransform.mRotation.z);

  return (translationMatrix * rotationMatrix * scalarMatrix);
}

/******************************************************************************/
Mat4 CalculateViewMatrix(const Kuma3D::Camera& aCamera,
                         const Kuma3D::Transform& aTransform)
{
  // To calculate the view matrix, we need three things: a direction vector,
  // a right vector, and an up vector.

  // To calculate the direction vector, first we assume the camera is at
  // the origin (0.0, 0.0, 0.0) and looking forward along the z-axis.
  Vec3 directionVector = Vec3(0.0, 0.0, 1.0);

  // To get the true direction vector, we need to calculate the rotation
  // transformation matrix of the camera and use it to rotate the default
  // direction vector.
  auto rotationMatrix = Rotate(Vec3(1.0, 0.0, 0.0), aTransform.mRotation.x);
  rotationMatrix = rotationMatrix * Rotate(Vec3(0.0, 1.0, 0.0), aTransform.mRotation.y);
  rotationMatrix = rotationMatrix * Rotate(Vec3(0.0, 0.0, 1.0), aTransform.mRotation.z);
  directionVector = rotationMatrix * directionVector;

  // Now that we have a direction vector, we can calculate a right vector by
  // taking the cross product of the direction vector and a default up vector
  // (in this case, (0.0, 1.0, 0.0)).
  auto rightVector = Cross(Vec3(0.0, 1.0, 0.0), directionVector);

  // Finally, we can construct the view matrix.
  return View(directionVector, rightVector, aTransform.mPosition);
}

/******************************************************************************/
Mat4 CalculateProjectionMatrix(const Kuma3D::CoordinateSystem& aSystem,
                               const Kuma3D::Camera& aCamera)
{
  Mat4 projectionMatrix;

  switch(aSystem)
  {
    case Kuma3D::CoordinateSystem::eSCREEN_SPACE:
    {
      // Create an orthographic projection.
      projectionMatrix = Orthographic(aCamera);
      break;
    }
    case Kuma3D::CoordinateSystem::eWORLD_SPACE:
    {
      // Create a perspective projection.
      projectionMatrix = Perspective(aCamera);
      break;
    }
  }

  return projectionMatrix;
}

/******************************************************************************/
RenderSystem::RenderSystem()
  : System()
{
  GamePendingExit.Connect(mObserver, [this](double aTime)
  {
    this->HandleGamePendingExit(aTime);
  });
}

/******************************************************************************/
void RenderSystem::Initialize(Scene& aScene)
{
  // Register the Mesh and Transform components.
  if(!aScene.IsComponentTypeRegistered<Mesh>())
  {
    aScene.RegisterComponentType<Mesh>();
  }

  if(!aScene.IsComponentTypeRegistered<Transform>())
  {
    aScene.RegisterComponentType<Transform>();
  }

  // Set the signature to care about entities with Meshes and Transforms.
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Mesh>()] = true;
  signature[aScene.GetComponentIndex<Transform>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void RenderSystem::Operate(Scene& aScene, double aTime)
{
  // First, clear the window of the last frame.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Next, separate the entities into two lists: one for transparent entities
  // and one for opaque entities.
  std::vector<Entity> transparentEntities;
  std::vector<Entity> opaqueEntities;
  for(const auto& entity : GetEntities())
  {
    auto& entityMesh = aScene.GetComponentForEntity<Mesh>(entity);
    if(entityMesh.mDirty)
    {
      // If the mesh has the dirty flag set, update the OpenGL buffer
      // with the new vertices and indices.
      UpdateBuffersForEntity(entity, entityMesh.mVertices, entityMesh.mIndices);
      entityMesh.mDirty = false;
    }

    if(entityMesh.mHasTransparency)
    {
      transparentEntities.emplace_back(entity);
    }
    else
    {
      opaqueEntities.emplace_back(entity);
    }
  }

  // Finally, for each camera, draw each entity. The opaque entities are drawn
  // first, and the transparent entities are drawn second.
  for(const auto& cameraEntity : mCameraEntities)
  {
    DrawEntities(aScene, cameraEntity, opaqueEntities);
    DrawEntities(aScene, cameraEntity, transparentEntities);
  }
}

/******************************************************************************/
Entity RenderSystem::CreateCamera(Scene& aScene)
{
  if(!aScene.IsComponentTypeRegistered<Camera>())
  {
    aScene.RegisterComponentType<Camera>();
  }

  if(!aScene.IsComponentTypeRegistered<Transform>())
  {
    aScene.RegisterComponentType<Transform>();
  }

  auto cameraEntity = aScene.CreateEntity();
  aScene.AddComponentToEntity<Camera>(cameraEntity);
  aScene.AddComponentToEntity<Transform>(cameraEntity);

  mCameraEntities.emplace_back(cameraEntity);
  return mCameraEntities.back();
}

/******************************************************************************/
void RenderSystem::HandleEntityBecameEligible(const Entity& aEntity)
{
  mVertexArrayMap[aEntity] = 0;
  mVertexBufferMap[aEntity] = 0;
  mElementBufferMap[aEntity] = 0;

  glGenVertexArrays(1, &mVertexArrayMap[aEntity]);
  glGenBuffers(1, &mVertexBufferMap[aEntity]);
  glGenBuffers(1, &mElementBufferMap[aEntity]);
}

/******************************************************************************/
void RenderSystem::HandleEntityBecameIneligible(const Entity& aEntity)
{
  glDeleteVertexArrays(1, &mVertexArrayMap[aEntity]);
  glDeleteBuffers(1, &mVertexBufferMap[aEntity]);
  glDeleteBuffers(1, &mElementBufferMap[aEntity]);

  mVertexArrayMap.erase(aEntity);
  mVertexBufferMap.erase(aEntity);
  mElementBufferMap.erase(aEntity);
}

/******************************************************************************/
void RenderSystem::HandleGamePendingExit(double aTime)
{
  for(const auto& vertexArrayPair : mVertexArrayMap)
  {
    glDeleteVertexArrays(1, &vertexArrayPair.second);
  }
  mVertexArrayMap.clear();

  for(const auto& vertexBufferPair : mVertexBufferMap)
  {
    glDeleteBuffers(1, &vertexBufferPair.second);
  }
  mVertexBufferMap.clear();

  for(const auto& elementBufferPair : mElementBufferMap)
  {
    glDeleteVertexArrays(1, &elementBufferPair.second);
  }
  mElementBufferMap.clear();
}

/******************************************************************************/
void RenderSystem::DrawEntities(Scene& aScene,
                                const Entity& aCamera,
                                const std::vector<Entity>& aEntities)
{
  auto& camera = aScene.GetComponentForEntity<Camera>(aCamera);
  auto& cameraTransform = aScene.GetComponentForEntity<Transform>(aCamera);

  glViewport(0.0, 0.0, camera.mViewportX, camera.mViewportY);

  auto viewMatrix = CalculateViewMatrix(camera, cameraTransform);

  for(const auto& entity : aEntities)
  {
    auto& entityMesh = aScene.GetComponentForEntity<Mesh>(entity);
    auto& entityTransform = aScene.GetComponentForEntity<Transform>(entity);

    // Bind each texture on this mesh.
    for(int i = 0; i < entityMesh.mTextures.size(); ++i)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, entityMesh.mTextures[i]);
    }

    // For each shader on this mesh, render it.
    for(const auto& shader : entityMesh.mShaders)
    {
      glUseProgram(shader);

      // Set the model matrix.
      if(ShaderLoader::IsUniformDefined(shader, "modelMatrix"))
      {
        // If the Entity's Transform component has a parent Transform,
        // calculate the model matrix for the parent.
        Mat4 parentMatrix;
        if(entityTransform.mUseParent)
        {
          auto& parentTransform = aScene.GetComponentForEntity<Transform>(entityTransform.mParent);
          parentMatrix = CalculateModelMatrix(parentTransform);
        }

        auto matrix = parentMatrix * CalculateModelMatrix(entityTransform);
        ShaderLoader::SetMat4(shader, "modelMatrix", matrix);
      }

      // Set the view matrix.
      if(ShaderLoader::IsUniformDefined(shader, "viewMatrix"))
      {
        ShaderLoader::SetMat4(shader, "viewMatrix", viewMatrix);
      }

      // Set the projection matrix.
      if(ShaderLoader::IsUniformDefined(shader, "projectionMatrix"))
      {
        auto matrix = CalculateProjectionMatrix(entityMesh.mSystem, camera);
        ShaderLoader::SetMat4(shader, "projectionMatrix", matrix);
      }

      // Draw the mesh.
      glBindVertexArray(mVertexArrayMap[entity]);
      glDrawElements(static_cast<GLenum>(entityMesh.mRenderMode),
                     entityMesh.mIndices.size(),
                     GL_UNSIGNED_INT,
                     0);
      glBindVertexArray(0);
    }
  }
}

/******************************************************************************/
void RenderSystem::UpdateBuffersForEntity(const Entity& aEntity,
                                          const std::vector<MeshVertex>& aVertices,
                                          const std::vector<unsigned int>& aIndices)
{
  // Bind the vertex array.
  glBindVertexArray(mVertexArrayMap[aEntity]);

  // Copy the vertex data into the vertex buffer.
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferMap[aEntity]);
  glBufferData(GL_ARRAY_BUFFER,
               aVertices.size() * sizeof(MeshVertex),
               &aVertices[0],
               GL_STATIC_DRAW);

  // Configure the vertex position attributes.
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(MeshVertex),
                        (void*)(0));

  // Configure the vertex color attributes.
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(MeshVertex),
                        (void*)(offsetof(MeshVertex, mColor)));

  // Configure the vertex texture coordinates.
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(MeshVertex),
                        (void*)(offsetof(MeshVertex, mTexCoords)));

  // Copy the index data into the element buffer.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBufferMap[aEntity]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               aIndices.size() * sizeof(unsigned int),
               &aIndices[0],
               GL_STATIC_DRAW);

  // Unbind the vertex array.
  glBindVertexArray(0);
}

} // namespace Kuma3D
