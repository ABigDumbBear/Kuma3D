#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "Vec3.hpp"
#include "IDGenerator.hpp"

namespace Kuma3D {

/**
 * An enumeration for all supported OpenGL draw modes.
 * Values correspond to OpenGL definitions and can be used
 * with glDrawElements.
 */
enum class RenderMode
{
  ePOINTS     = 0x0000,
  eLINES      = 0x0001,
  eLINE_LOOP  = 0x0002,
  eLINE_STRIP = 0x0003,
  eTRIANGLES  = 0x0004
};

/**
 * An enumeration for all supported coordinate systems.
 * World space uses a perspective transformation matrix,
 * while screen space uses an orthographic transformation matrix.
 */
enum class CoordinateSystem
{
  eWORLD_SPACE,
  eSCREEN_SPACE
};

/**
 * A struct that contains all data needed to render a vertex.
 *
 * A vector of each MeshVertex is passed into the active Shader
 * object each time Render() is called; there they can be accessed
 * at the corresponding input location.
 *
 * The position value of a MeshVertex is assumed to be in model space.
 */
struct MeshVertex
{
  Vec3 mPosition; // location = 0
  Vec3 mColor; // location = 1
  Vec3 mNormal; // location = 2
  float mTexCoords[2] { 0.0, 0.0 }; // location = 3
};

/**
 * A struct that defines how a Mesh responds to light.
 */
struct Material
{
  Vec3 mAmbientColor { 1, 1, 1 };
  Vec3 mDiffuseColor { 1, 1, 1 };
  Vec3 mSpecularColor { 1, 1, 1 };
  float mShininess { 1 };
};

/**
 * The actual Mesh component data.
 */
struct Mesh
{
  RenderMode mRenderMode   { RenderMode::eTRIANGLES };
  CoordinateSystem mSystem { CoordinateSystem::eWORLD_SPACE };

  std::vector<MeshVertex> mVertices;
  std::vector<unsigned int> mIndices;
  std::vector<ID> mTextures;
  std::vector<ID> mShaders;

  Material mMaterial;

  bool mUseDepthTesting { true };

  bool mHasTransparency { false };
  bool mDirty { false };
};

} // namespace Kuma3D

#endif
