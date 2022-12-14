#include "ModelLoader.hpp"

#include <stdexcept>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Model.hpp"
#include "Transform.hpp"

namespace Kuma3D {

/******************************************************************************/
IDGenerator ModelLoader::mIDGenerator;
std::map<std::string, ID> ModelLoader::mModelFileMap;
std::map<ID, std::vector<Mesh>> ModelLoader::mModelMap;

std::string ModelLoader::mWorkingDirectory;

TextureStorageFormat ModelLoader::mFormat;
TextureWrapOption ModelLoader::mWrapOption;
TextureFilterOption ModelLoader::mFilterOption;

/******************************************************************************/
ID ModelLoader::LoadModel(const std::string& aFilePath,
                          TextureStorageFormat aFormat,
                          TextureWrapOption aWrapOption,
                          TextureFilterOption aFilterOption)
{
  ID modelID = 0;

  // Set the working directory.
  mWorkingDirectory = aFilePath.substr(0, aFilePath.find_last_of("/\\"));

  // Store the texture options.
  mFormat = aFormat;
  mWrapOption = aWrapOption;
  mFilterOption = aFilterOption;

  // Don't load the same model twice.
  auto foundID = mModelFileMap.find(aFilePath);
  if(foundID != mModelFileMap.end())
  {
    modelID = foundID->second;
  }
  else
  {
    // Generate an ID for the model.
    modelID = mIDGenerator.GenerateID();

    Assimp::Importer importer;
    auto modelScene = importer.ReadFile(aFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
    ProcessNode(modelID, *modelScene->mRootNode, *modelScene);

    mModelFileMap.emplace(aFilePath, modelID);
  }

  return modelID;
}

/******************************************************************************/
Entity ModelLoader::CreateModel(ID aID, Scene& aScene)
{
  auto foundModel = mModelMap.find(aID);
  if(foundModel == mModelMap.end())
  {
    std::stringstream error;
    error << "No model with ID " << aID << " exists!";
    throw(std::invalid_argument(error.str()));
  }

  // Register the appropriate components if they aren't already.
  if(!aScene.IsComponentTypeRegistered<Mesh>())
  {
    aScene.RegisterComponentType<Mesh>();
  }

  if(!aScene.IsComponentTypeRegistered<Model>())
  {
    aScene.RegisterComponentType<Model>();
  }

  if(!aScene.IsComponentTypeRegistered<Transform>())
  {
    aScene.RegisterComponentType<Transform>();
  }

  // Create a new Entity for the model.
  auto modelEntity = aScene.CreateEntity();
  Model modelComponent;

  // For each mesh in the model, create a new Entity and attach the mesh.
  for(const auto& mesh : foundModel->second)
  {
    auto meshEntity = aScene.CreateEntity();

    auto meshCopy = mesh;
    aScene.AddComponentToEntity<Mesh>(meshEntity, meshCopy);

    Transform meshTransform;
    meshTransform.mParent = modelEntity;
    meshTransform.mUseParent = true;
    aScene.AddComponentToEntity<Transform>(meshEntity, meshTransform);

    // Keep track of the child Entity.
    modelComponent.mChildren.emplace_back(meshEntity);
  }

  aScene.AddComponentToEntity<Model>(modelEntity, modelComponent);
  aScene.AddComponentToEntity<Transform>(modelEntity);

  return modelEntity;
}

/******************************************************************************/
void ModelLoader::ProcessNode(const ID& aID, aiNode& aNode, const aiScene& aScene)
{
  // Process each mesh in this node.
  for(int i = 0; i < aNode.mNumMeshes; ++i)
  {
    auto mesh = aScene.mMeshes[aNode.mMeshes[i]];
    ProcessMesh(aID, *mesh, aScene);
  }

  // Process each of this node's children.
  for(int i = 0; i < aNode.mNumChildren; ++i)
  {
    ProcessNode(aID, *aNode.mChildren[i], aScene);
  }
}

/******************************************************************************/
void ModelLoader::ProcessMesh(const ID& aID, aiMesh& aMesh, const aiScene& aScene)
{
  Mesh mesh;

  // Retrieve the vertex data.
  for(int i = 0; i < aMesh.mNumVertices; ++i)
  {
    MeshVertex vertex;

    // Default the vertex color to white.
    vertex.mColor = Vec3(1.0, 1.0, 1.0);

    // Retrieve the position data.
    vertex.mPosition = Vec3(aMesh.mVertices[i].x,
                            aMesh.mVertices[i].y,
                            aMesh.mVertices[i].z);

    // Retrieve the texture coordinates.
    // TODO: for now, this only retrieves the texture coordinates for the first
    // texture, since MeshVertex only supports one set of coordinates. It might
    // be worth addressing this in the future.
    auto texCoords = aMesh.mTextureCoords[0];
    if(texCoords != nullptr)
    {
      vertex.mTexCoords[0] = texCoords[i].x;
      vertex.mTexCoords[1] = texCoords[i].y;
    }

    // Load the textures themselves, if there are any.
    if(aMesh.mMaterialIndex >= 0)
    {
      auto material = aScene.mMaterials[aMesh.mMaterialIndex];

      // Load each texture of each type for the material.
      auto textureIDs = GetTexturesForMaterial(*material, aiTextureType_NONE);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_DIFFUSE);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_SPECULAR);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_AMBIENT);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_EMISSIVE);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_HEIGHT);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_NORMALS);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_SHININESS);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_OPACITY);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_DISPLACEMENT);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_LIGHTMAP);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_REFLECTION);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
      textureIDs = GetTexturesForMaterial(*material, aiTextureType_UNKNOWN);
      for(const auto& id : textureIDs)
      {
        mesh.mTextures.emplace_back(id);
      }
    }

    mesh.mVertices.emplace_back(vertex);
  }

  // Retrieve the index data.
  for(int i = 0; i < aMesh.mNumFaces; ++i)
  {
    auto face = aMesh.mFaces[i];
    for(int j = 0; j < face.mNumIndices; ++j)
    {
      mesh.mIndices.emplace_back(face.mIndices[j]);
    }
  }

  mesh.mDirty = true;

  mModelMap[aID].emplace_back(mesh);
}

/******************************************************************************/
std::vector<ID> ModelLoader::GetTexturesForMaterial(const aiMaterial& aMaterial,
                                                    const aiTextureType& aType)
{
  std::vector<ID> textureIDs;

  for(int i = 0; i < aMaterial.GetTextureCount(aType); ++i)
  {
    aiString str;
    aMaterial.GetTexture(aType, i, &str);

    std::stringstream texturePath;
    texturePath << mWorkingDirectory << "/" << str.C_Str();
    auto textureID = TextureLoader::LoadTextureFromFile(texturePath.str(),
                                                        mFormat,
                                                        mWrapOption,
                                                        mFilterOption);
    textureIDs.emplace_back(textureID);
  }

  return textureIDs;
}

} // namespace Kuma3D
