#include "ModelLoader.hpp"

#include <stdexcept>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Kuma3D {

/******************************************************************************/
IDGenerator ModelLoader::mIDGenerator;
std::map<std::string, ID> ModelLoader::mModelFileMap;
std::map<ID, std::vector<Mesh>> ModelLoader::mModelMap;

/******************************************************************************/
ID ModelLoader::LoadModel(const std::string& aFilePath)
{
  ID modelID = 0;

  // Don't load the same model twice.
  auto foundID = mModelFileMap.find(aFilePath);
  if(foundID != mModelFileMap.end())
  {
    modelID = foundID->second;
  }
  else
  {
    // Generate an ID for the model.
    auto newID = mIDGenerator.GenerateID();

    Assimp::Importer importer;
    auto modelScene = importer.ReadFile(aFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    ProcessNode(newID, *modelScene->mRootNode, *modelScene);
  }

  return modelID;
}

/******************************************************************************/
std::vector<Mesh> ModelLoader::GetModel(const ID& aID)
{
  auto foundModel = mModelMap.find(aID);
  if(foundModel == mModelMap.end())
  {
    std::stringstream error;
    error << "No model with ID " << aID << " exists!";
    throw(std::invalid_argument(error.str()));
  }

  return foundModel->second;
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

  mModelMap[aID].emplace_back(mesh);
}

} // namespace Kuma3D
