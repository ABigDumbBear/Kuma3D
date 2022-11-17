#ifndef TEXTURELOADER_HPP
#define TEXTURELOADER_HPP

#include <string>
#include <map>

#include <GL/glew.h>

#include "Types.hpp"

namespace Kuma3D {

using TextureDimensions = std::pair<int, int>;

/**
 * Represents each supported texture storage format.
 */
enum class TextureStorageFormat
{
  eR,
  eRGB,
  eRGBA
};

/**
 * Represents each supported texture wrapping option.
 */
enum class TextureWrapOption
{
  eREPEAT,
  eMIRRORED_REPEAT,
  eCLAMP_TO_EDGE,
  eCLAMP_TO_BORDER
};

/**
 * Represents each supported texture filtering option.
 */
enum class TextureFilterOption
{
  eNEAREST,
  eLINEAR
};

/**
 * A static class that handles loading and manipulating OpenGL textures.
 */
class TextureLoader
{
  public:

    /**
     * Loads the image data from a given file and creates an OpenGL texture
     * for it. If the file has already been loaded, this function does nothing
     * but return the ID of the corresponding OpenGL texture.
     *
     * @param aFilePath The path to an image file.
     * @param aFormat The format to store the texture in. By default, this is RGBA.
     * @param aTextureWrapOption What to do with texture coordinates outside the
     *                           range of 0.0 to 1.0. By default, this is REPEAT.
     * @param aTextureFilterOption How OpenGL will calculate texel color. By
     *                             default, it will blend the colors of the
     *                             surrounding texels.
     * @return The ID of the stored OpenGL texture.
     */
    static ID LoadTextureFromFile(const std::string& aFilePath,
                                  TextureStorageFormat aFormat = TextureStorageFormat::eRGBA,
                                  TextureWrapOption aWrapOption = TextureWrapOption::eREPEAT,
                                  TextureFilterOption aFilterOption = TextureFilterOption::eLINEAR);

    /**
     * Creates an OpenGL texture from preloaded image data.
     *
     * @param aData A pointer to the image data.
     * @param aWidth The width of the image.
     * @param aHeight The height of the image.
     * @param aFormat The format to store the texture in. By default, this is RGBA.
     * @param aTextureWrapOption What to do with texture coordinates outside the
     *                           range of 0.0 to 1.0. By default, this is REPEAT.
     * @param aTextureFilterOption How OpenGL will calculate texel color. By
     *                             default, it will blend the colors of the
     *                             surrounding texels.
     * @return The ID of the stored OpenGL texture.
     */
    static ID LoadTextureFromData(unsigned char* aData,
                                  unsigned int aWidth,
                                  unsigned int aHeight,
                                  TextureStorageFormat aFormat = TextureStorageFormat::eRGBA,
                                  TextureWrapOption aWrapOption = TextureWrapOption::eREPEAT,
                                  TextureFilterOption aFilterOption = TextureFilterOption::eNEAREST);

    /**
     * Adds image data to a texture. Remember that the texture's origin
     * is at the bottom left of the image. The x-axis is positive to the
     * right, and the y-axis is positive in the up direction.
     *
     * @param aID The ID of the texture.
     * @param aData A pointer to the image data.
     * @param aXOffset The x coordinate to place the image data at
     *                 in the texture.
     * @param aYOffset The y coordinate to place the image data at
     *                 in the texture.
     * @param aWidth The width of the image data.
     * @param aHeight The height of the image data.
     * @param aFormat The format to store the texture in. By default, this is RGBA.
     */
    static void AddSubImageData(const ID& aID,
                                unsigned char* aData,
                                unsigned int aXOffset,
                                unsigned int aYOffset,
                                unsigned int aWidth,
                                unsigned int aHeight,
                                TextureStorageFormat aFormat = TextureStorageFormat::eRGBA);

    /**
     * Retrieves the dimensions of a loaded texture. The first entry is the
     * width, and the second entry is the height.
     *
     * @param aID The ID of the texture.
     * @return The dimensions of the texture.
     */
    static TextureDimensions GetTextureDimensions(const ID& aID);

    /**
     * Unloads all OpenGL textures. If you load any textures, be sure
     * to call this before the program terminates to avoid memory leaks.
     */
    static void UnloadTextures();

  private:

    // Maps file names to OpenGL texture IDs.
    static std::map<std::string, ID> mTextureMap;

    // Maps texture IDs to width and height values.
    static std::map<ID, TextureDimensions> mTextureDimensionMap;
};

} // namespace Kuma3D

#endif
