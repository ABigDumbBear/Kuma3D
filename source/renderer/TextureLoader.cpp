#include "TextureLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/glew.h>

#include <iostream>

namespace Kuma3D {

std::map<std::string, ID> TextureLoader::mTextureMap;

/*****************************************************************************/
ID TextureLoader::LoadTextureFromFile(const std::string& aFilePath,
                                      GLint aFormat)
{
  // Only load the texture if it hasn't already been loaded.
  auto foundTexture = mTextureMap.find(aFilePath);
  if(foundTexture == mTextureMap.end())
  {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    auto data = stbi_load(aFilePath.c_str(), &width, &height, &channels, 0);
    auto textureID = LoadTextureFromData(data, width, height, aFormat);
    stbi_image_free(data);

    mTextureMap.emplace(aFilePath, textureID);
  }

  return mTextureMap[aFilePath];
}

/*****************************************************************************/
ID TextureLoader::LoadTextureFromData(unsigned char* aData,
                                      unsigned int aWidth,
                                      unsigned int aHeight,
                                      GLint aFormat)
{
  // Generate an OpenGL texture.
  ID textureID = 0;
  glGenTextures(1, &textureID);

  // Set default texture wrapping and filtering options.
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Copy the image data into the currently bound texture.
  glTexImage2D(GL_TEXTURE_2D,
               0,
               aFormat,
               aWidth,
               aHeight,
               0,
               aFormat,
               GL_UNSIGNED_BYTE,
               aData);

  // Create a mipmap for this texture; used on small/far away objects.
  glGenerateMipmap(GL_TEXTURE_2D);

  return textureID;
}

/*****************************************************************************/
void TextureLoader::UnloadTextures()
{
  for(const auto& texturePair : mTextureMap)
  {
    glDeleteTextures(1, &texturePair.second);
  }

  mTextureMap.clear();
}

/*****************************************************************************/
void TextureLoader::AddSubImageData(const ID& aID,
                                    unsigned char* aData,
                                    unsigned int aXOffset,
                                    unsigned int aYOffset,
                                    unsigned int aWidth,
                                    unsigned int aHeight,
                                    GLint aFormat)
{
  // Bind texture for use.
  glBindTexture(GL_TEXTURE_2D, aID);

  // Add the image data to the existing texture.
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  aXOffset,
                  aYOffset,
                  aWidth,
                  aHeight,
                  aFormat,
                  GL_UNSIGNED_BYTE,
                  aData);

  // Recreate the texture mipmap with the new data.
  glGenerateMipmap(GL_TEXTURE_2D);
}

} // namespace Kuma3D
