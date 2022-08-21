#ifndef TEXTURELOADER_HPP
#define TEXTURELOADER_HPP

#include <string>
#include <map>

#include <GL/glew.h>

#include "Types.hpp"

namespace Kuma3D {

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
     * @param aFormat The format to store the texture in. By default, this
     *                is GL_RGBA.
     * @return The ID of the stored OpenGL texture.
     */
    static ID LoadTextureFromFile(const std::string& aFilePath,
                                  GLint aFormat = GL_RGBA);

    /**
     * Creates an OpenGL texture from preloaded image data.
     *
     * @param aData A pointer to the image data.
     * @param aWidth The width of the image.
     * @param aHeight The height of the image.
     * @param aFormat The format to store the texture in. By default, this
     *                is GL_RGBA.
     * @return The ID of the stored OpenGL texture.
     */
    static ID LoadTextureFromData(unsigned char* aData,
                                  unsigned int aWidth,
                                  unsigned int aHeight,
                                  GLint aFormat = GL_RGBA);

    /**
     * Unloads all OpenGL textures. If you load any textures, be sure
     * to call this before the program terminates to avoid memory leaks.
     */
    static void UnloadTextures();

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
     * @param aFormat The format to store the texture in. By default, this
     *                is GL_RGBA.
     */
    static void AddSubImageData(const ID& aID,
                                unsigned char* aData,
                                unsigned int aXOffset,
                                unsigned int aYOffset,
                                unsigned int aWidth,
                                unsigned int aHeight,
                                GLint aFormat = GL_RGBA);

  private:

    // Maps file names to OpenGL texture IDs.
    static std::map<std::string, ID> mTextureMap;
};

} // namespace Kuma3D

#endif
