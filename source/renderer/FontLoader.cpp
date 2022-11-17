#include "FontLoader.hpp"

#include <sstream>
#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>

#include "TextureLoader.hpp"

namespace Kuma3D {

bool FontLoader::mInitialized = false;
FT_Library FontLoader::mLibrary;

std::map<std::string, FontFaceIdentifier> FontLoader::mFontFileMap;
std::map<FontFaceIdentifier, FT_Face> FontLoader::mFontFaceMap;
std::map<FontTextureIdentifier, std::map<char, GlyphInfo>> FontLoader::mFontGlyphMap;
std::map<FontTextureIdentifier, ID> FontLoader::mFontTextureMap;

/******************************************************************************/
void FontLoader::LoadFontFromFile(const std::string& aFilePath)
{
  if(!mInitialized)
  {
    auto error = FT_Init_FreeType(&mLibrary);
    if(error== 0)
    {
      mInitialized = true;
    }
  }

  if(mInitialized)
  {
    // If the font has already been loaded, don't load it again.
    auto foundFont = mFontFileMap.find(aFilePath);
    if(foundFont == mFontFileMap.end())
    {
      FT_Face newFace;
      auto error = FT_New_Face(mLibrary, aFilePath.c_str(), 0, &newFace);
      if(error == 0)
      {
        FontFaceIdentifier fontID;
        fontID.mFamily = newFace->family_name;
        fontID.mStyle = newFace->style_name;

        mFontFileMap.emplace(aFilePath, fontID);
        mFontFaceMap.emplace(fontID, newFace);
      }
    }
  }
}

/******************************************************************************/
ID FontLoader::LoadTextureForFont(const FontTextureIdentifier& aFontTextureID)
{
  auto foundFace = mFontFaceMap.find(aFontTextureID.mFace);
  if(foundFace == mFontFaceMap.end())
  {
    std::stringstream error;
    error << "The font from family " << aFontTextureID.mFace.mFamily;
    error << " with style " << aFontTextureID.mFace.mStyle;
    error << " hasn't been loaded yet!";
    throw std::invalid_argument(error.str());
  }

  ID textureID = 0;
  auto foundTexture = mFontTextureMap.find(aFontTextureID);
  if(foundTexture != mFontTextureMap.end())
  {
    textureID = foundTexture->second;
  }
  else
  {
    // First, set the size of the font face.
    auto face = foundFace->second;
    FT_Set_Pixel_Sizes(face, 0, aFontTextureID.mSize);

    // Next, determine the necessary height and width of the font atlas.
    // The atlas will contain the first 128 ASCII characters laid out
    // horizontally from left to right. Each character will be separated
    // horizontally by a pixel amount equal to mTexturePadding. They
    // will also be separated from the top and bottom of the texture by
    // the same pixel amount.
    unsigned int atlasHeight = 0;
    unsigned int atlasWidth = mTexturePadding;
    for(unsigned int i = 32; i < 128; ++i)
    {
      auto error = FT_Load_Char(face, i, FT_LOAD_RENDER);
      if(error)
      {
        // If the font doesn't support this character, skip it.
        continue;
      }

      atlasWidth += face->glyph->bitmap.width;
      atlasWidth += (mTexturePadding * 2);
      atlasHeight = std::max(atlasHeight, face->glyph->bitmap.rows);
    }

    atlasHeight += (mTexturePadding * 2);

    // Use the height and width to create an empty texture.
    // The load format is specified as TextureLoadFormat::eR because Freetype stores
    // image data as an 8-bit image, where each color is a single byte.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    textureID = TextureLoader::LoadTextureFromData(nullptr,
                                                   atlasWidth,
                                                   atlasHeight,
                                                   TextureStorageFormat::eR);

    // Now that we have an empty texture, fill it with the image data
    // for each of the first 128 ASCII characters.
    unsigned int xOffset = mTexturePadding;
    for(unsigned int i = 32; i < 128; ++i)
    {
      auto error = FT_Load_Char(face, i, FT_LOAD_RENDER);
      if(error)
      {
        // If the font doesn't support this character, skip it.
        continue;
      }

      // Add the glyph data to the font atlas.
      TextureLoader::AddSubImageData(textureID,
                                     face->glyph->bitmap.buffer,
                                     xOffset,
                                     mTexturePadding,
                                     face->glyph->bitmap.width,
                                     face->glyph->bitmap.rows,
                                     TextureStorageFormat::eR);

      xOffset += face->glyph->bitmap.width;
      xOffset += (mTexturePadding * 2);
    }

    // After loading the texture data into memory, reset OpenGL's
    // unpack alignment to the default (4).
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);

    // Next, calculate a GlyphMap for this font at this size.
    CreateGlyphMapForFont(aFontTextureID);

    // Finally, keep track of the texture ID.
    mFontTextureMap.emplace(aFontTextureID, textureID);
  }

  return textureID;
}

/******************************************************************************/
void FontLoader::UnloadFontsAndTextures()
{
  // Deallocate all the loaded font faces.
  for(auto& fontFacePair : mFontFaceMap)
  {
    FT_Done_Face(fontFacePair.second);
  }
  mFontFaceMap.clear();

  // Deallocate all the OpenGL textures.
  for(auto& fontTexturePair : mFontTextureMap)
  {
    glDeleteTextures(1, &fontTexturePair.second);
  }
  mFontTextureMap.clear();

  // Remove all other font data.
  mFontFileMap.clear();
  mFontGlyphMap.clear();

  // Clean up Freetype.
  FT_Done_FreeType(mLibrary);
  mInitialized = false;
}

/******************************************************************************/
void FontLoader::UpdateMeshToDisplayText(Mesh& aMesh,
                                         const std::string& aText,
                                         const FontTextureIdentifier& aFontTextureID)
{
  auto foundFace = mFontFaceMap.find(aFontTextureID.mFace);
  auto foundGlyphMap = mFontGlyphMap.find(aFontTextureID);
  auto foundTexture = mFontTextureMap.find(aFontTextureID);
  if(foundFace != mFontFaceMap.end() &&
     foundGlyphMap != mFontGlyphMap.end() &&
     foundTexture != mFontTextureMap.end())
  {
    auto face = foundFace->second;
    auto glyphMap = foundGlyphMap->second;

    // First, remove any vertices, indices, and/or textures on the given Mesh.
    aMesh.mVertices.clear();
    aMesh.mIndices.clear();
    aMesh.mTextures.clear();

    // Next, retrieve the total height and width of the font atlas.
    auto dimensions = TextureLoader::GetTextureDimensions(foundTexture->second);
    float atlasWidth = dimensions.first;
    float atlasHeight = dimensions.second;

    // Next, for each character in the text, use the loaded glyph information
    // (if available) to create a vertex quad for that glyph.
    MeshVertex vertex;
    int xPos = 0;
    int yPos = 0;
    int width = 0;
    int height = 0;
    int numLines = 0;

    for(const auto& character : aText)
    {
      auto glyph = glyphMap.find(character);
      if(glyph != glyphMap.end())
      {
        xPos += glyph->second.mBearingX;
        yPos = -1 * (glyph->second.mHeight - glyph->second.mBearingY);
        width = glyph->second.mWidth;
        height = glyph->second.mHeight;

        yPos -= numLines * (face->size->metrics.height / 64);

        // Create a quad that contains this character.
        float textureX = 0.0;
        float textureY = 0.0;

        // Bottom Left
        textureX = glyph->second.mOffsetX / atlasWidth;
        vertex.mPosition = Vec3(xPos, yPos + height, 0);
        vertex.mTexCoords[0] = textureX;
        vertex.mTexCoords[1] = textureY;
        aMesh.mVertices.emplace_back(vertex);

        // Top Left
        textureY = glyph->second.mHeight / atlasHeight;
        vertex.mPosition = Vec3(xPos, yPos, 0);
        vertex.mTexCoords[0] = textureX;
        vertex.mTexCoords[1] = textureY;
        aMesh.mVertices.emplace_back(vertex);

        // Top Right
        textureX = (glyph->second.mOffsetX + glyph->second.mWidth) / atlasWidth;
        vertex.mPosition = Vec3(xPos + width, yPos, 0);
        vertex.mTexCoords[0] = textureX;
        vertex.mTexCoords[1] = textureY;
        aMesh.mVertices.emplace_back(vertex);

        // Bottom Left
        textureX = glyph->second.mOffsetX / atlasWidth;
        textureY = 0.0;
        vertex.mPosition = Vec3(xPos, yPos + height, 0);
        vertex.mTexCoords[0] = textureX;
        vertex.mTexCoords[1] = textureY;
        aMesh.mVertices.emplace_back(vertex);

        // Top Right
        textureX = (glyph->second.mOffsetX + glyph->second.mWidth) / atlasWidth;
        textureY = glyph->second.mHeight / atlasHeight;
        vertex.mPosition = Vec3(xPos + width, yPos, 0);
        vertex.mTexCoords[0] = textureX;
        vertex.mTexCoords[1] = textureY;
        aMesh.mVertices.emplace_back(vertex);

        // Bottom Right
        textureY = 0.0;
        vertex.mPosition = Vec3(xPos + width, yPos + height, 0);
        vertex.mTexCoords[0] = textureX;
        vertex.mTexCoords[1] = textureY;
        aMesh.mVertices.emplace_back(vertex);

        // Add indices to draw this quad.
        int indexCount = aMesh.mIndices.size();
        aMesh.mIndices.emplace_back(indexCount);
        aMesh.mIndices.emplace_back(indexCount + 1);
        aMesh.mIndices.emplace_back(indexCount + 2);
        aMesh.mIndices.emplace_back(indexCount + 3);
        aMesh.mIndices.emplace_back(indexCount + 4);
        aMesh.mIndices.emplace_back(indexCount + 5);

        // Update the x position for the next character.
        xPos += (glyph->second.mAdvance / 64);
      }
      else if(character == '\n')
      {
        xPos = 0;
        ++numLines;
      }
    }

    // Finally, add the font texture ID to the Mesh.
    aMesh.mTextures.emplace_back(foundTexture->second);

    aMesh.mHasTransparency = true;
    aMesh.mDirty = true;
  }
}

/******************************************************************************/
void FontLoader::CreateGlyphMapForFont(const FontTextureIdentifier& aFontTextureID)
{
  auto foundFace = mFontFaceMap.find(aFontTextureID.mFace);
  if(foundFace != mFontFaceMap.end())
  {
    auto face = foundFace->second;
    GlyphMap glyphMap;

    // For each of the first 128 ASCII characters, access their glyph information
    // and store it in the map.
    GlyphInfo info;
    unsigned int xOffset = 0;
    for(unsigned int i = 32; i < 128; ++i)
    {
      auto error = FT_Load_Char(face, i, FT_LOAD_RENDER);
      if(error)
      {
        // If the curent font doesn't support this character, skip it.
        continue;
      }

      // Store the glyph's measurements in the map.
      info.mWidth = face->glyph->bitmap.width + (mTexturePadding * 2);
      info.mHeight = face->glyph->bitmap.rows + (mTexturePadding * 2);
      info.mBearingX = face->glyph->bitmap_left;
      info.mBearingY = face->glyph->bitmap_top;
      info.mAdvance = face->glyph->advance.x;
      info.mOffsetX = xOffset;
      glyphMap.emplace((char)i, info);
      
      xOffset += info.mWidth;
    }

    // Finally, store the GlyphMap for later use.
    mFontGlyphMap.emplace(aFontTextureID, glyphMap);
  }
}

} // namespace Kuma3D
