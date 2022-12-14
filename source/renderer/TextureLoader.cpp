#include "TextureLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <GL/glew.h>

#include <sstream>
#include <stdexcept>

namespace Kuma3D {

std::map<std::string, ID> TextureLoader::mTextureMap;
std::map<ID, TextureDimensions> TextureLoader::mTextureDimensionMap;

/*****************************************************************************/
ID TextureLoader::LoadTextureFromFile(const std::string& aFilePath,
                                      TextureStorageFormat aFormat,
                                      TextureWrapOption aWrapOption,
                                      TextureFilterOption aFilterOption)
{
  // Only load the texture if it hasn't already been loaded.
  auto foundTexture = mTextureMap.find(aFilePath);
  if(foundTexture == mTextureMap.end())
  {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    auto data = stbi_load(aFilePath.c_str(),
                          &width,
                          &height,
                          &channels,
                          0);
    auto textureID = LoadTextureFromData(data,
                                         width,
                                         height,
                                         aFormat,
                                         aWrapOption,
                                         aFilterOption);
    stbi_image_free(data);

    mTextureMap.emplace(aFilePath, textureID);
  }

  return mTextureMap[aFilePath];
}

/*****************************************************************************/
ID TextureLoader::LoadTextureFromData(unsigned char* aData,
                                      unsigned int aWidth,
                                      unsigned int aHeight,
                                      TextureStorageFormat aFormat,
                                      TextureWrapOption aWrapOption,
                                      TextureFilterOption aFilterOption)
{
  // Generate an OpenGL texture.
  ID textureID = 0;
  glGenTextures(1, &textureID);

  GLint loadFormat, wrapOption, filterOption;
  switch(aFormat)
  {
    case TextureStorageFormat::eR: { loadFormat = GL_RED; break; }
    case TextureStorageFormat::eRGB: { loadFormat = GL_RGB; break; }
    case TextureStorageFormat::eRGBA: { loadFormat = GL_RGBA; break; }
  }

  switch(aWrapOption)
  {
    case TextureWrapOption::eREPEAT: { wrapOption = GL_REPEAT; break; }
    case TextureWrapOption::eMIRRORED_REPEAT: { wrapOption = GL_MIRRORED_REPEAT; break; }
    case TextureWrapOption::eCLAMP_TO_EDGE: { wrapOption = GL_CLAMP_TO_EDGE; break; }
    case TextureWrapOption::eCLAMP_TO_BORDER: { wrapOption = GL_CLAMP_TO_BORDER; break; }
  }

  switch(aFilterOption)
  {
    case TextureFilterOption::eNEAREST: { filterOption = GL_NEAREST; break; }
    case TextureFilterOption::eLINEAR: { filterOption = GL_LINEAR_MIPMAP_LINEAR; break; }
  }

  // Set default texture wrapping and filtering options.
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOption);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOption);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterOption);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterOption);

  // Copy the image data into the currently bound texture.
  glTexImage2D(GL_TEXTURE_2D,
               0,
               loadFormat,
               aWidth,
               aHeight,
               0,
               loadFormat,
               GL_UNSIGNED_BYTE,
               aData);

  // Create a mipmap for this texture; used on small/far away objects.
  glGenerateMipmap(GL_TEXTURE_2D);

  // Store the texture dimensions.
  TextureDimensions dimensions(aWidth, aHeight);
  mTextureDimensionMap.emplace(textureID, dimensions);

  return textureID;
}

/*****************************************************************************/
void TextureLoader::AddSubImageData(const ID& aID,
                                    unsigned char* aData,
                                    unsigned int aXOffset,
                                    unsigned int aYOffset,
                                    unsigned int aWidth,
                                    unsigned int aHeight,
                                    TextureStorageFormat aFormat)
{
  // Bind texture for use.
  glBindTexture(GL_TEXTURE_2D, aID);

  GLint loadFormat;
  switch(aFormat)
  {
    case TextureStorageFormat::eR: { loadFormat = GL_RED; break; }
    case TextureStorageFormat::eRGB: { loadFormat = GL_RGB; break; }
    case TextureStorageFormat::eRGBA: { loadFormat = GL_RGBA; break; }
  }

  // Add the image data to the existing texture.
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  aXOffset,
                  aYOffset,
                  aWidth,
                  aHeight,
                  loadFormat,
                  GL_UNSIGNED_BYTE,
                  aData);

  // Recreate the texture mipmap with the new data.
  glGenerateMipmap(GL_TEXTURE_2D);
}

/*****************************************************************************/
TextureDimensions TextureLoader::GetTextureDimensions(const ID& aID)
{
  auto foundTexture = mTextureDimensionMap.find(aID);
  if(foundTexture == mTextureDimensionMap.end())
  {
    std::stringstream error;
    error << "No texture with ID " << aID << " exists!" << std::endl;
    throw(std::invalid_argument(error.str()));
  }

  return mTextureDimensionMap[aID];
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

} // namespace Kuma3D
