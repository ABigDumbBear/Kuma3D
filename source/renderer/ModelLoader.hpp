#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP

#include <map>
#include <string>
#include <vector>

#include <assimp/material.h>
#include <assimp/scene.h>

#include "TextureLoader.hpp"

#include "IDGenerator.hpp"

#include "Mesh.hpp"

namespace Kuma3D {

/**
 * A static class that handles loading models from external files.
 */
class ModelLoader
{
  public:

    /**
     * Loads a model from the file at the given location. If a model at the
     * given location has already been loaded, this function does nothing but
     * return the ID of the corresponding list of Meshes.
     *
     * @param aFilePath The path to the model file.
     * @return The ID for a list of Meshes that represent the model.
     */
    static ID LoadModel(const std::string& aFilePath,
                        TextureStorageFormat aFormat = TextureStorageFormat::eRGBA,
                        TextureWrapOption aWrapOption = TextureWrapOption::eREPEAT,
                        TextureFilterOption aFilterOption = TextureFilterOption::eLINEAR);

    /**
     * Returns a list of Meshes that represent the model associated with
     * the given ID.
     *
     * @param aID The ID of the model to retrieve.
     * @return A list of Meshes that represent the model.
     */
    static std::vector<Mesh> GetModel(const ID& aID);

  private:

    /**
     * A recursive function that processes a node and all of its
     * children in an Assimp model tree.
     *
     * @param aID The ID to associate with the model being loaded.
     * @param aNode The node to process.
     * @param aScene The scene containing the nodes.
     */
    static void ProcessNode(const ID& aID, aiNode& aNode, const aiScene& aScene);

    /**
     * Retrieves mesh information from Assimp and stores it in a Kuma3D
     * Mesh component.
     *
     * @param aID The ID to associate with the model being loaded.
     * @param aMesh The Assimp mesh to process.
     * @param aScene The scene containing the mesh.
     */
    static void ProcessMesh(const ID& aID, aiMesh& aMesh, const aiScene& aScene);

    /**
     * Returns a list of texture IDs for each texture of a given type on a
     * given material.
     *
     * @param aMaterial The material to load textures for.
     * @param aType The type of texture to load.
     * @return A list of texture IDs for each texture used by the material.
     */
    static std::vector<ID> GetTexturesForMaterial(const aiMaterial& aMaterial,
                                                  const aiTextureType& aType);

    static IDGenerator mIDGenerator;
    static std::map<std::string, ID> mModelFileMap;
    static std::map<ID, std::vector<Mesh>> mModelMap;

    static std::string mWorkingDirectory;

    static TextureStorageFormat mFormat;
    static TextureWrapOption mWrapOption;
    static TextureFilterOption mFilterOption;
};

} // namespace Kuma3D

#endif
