#ifndef FONTLOADER_HPP
#define FONTLOADER_HPP

#include <map>
#include <string>
#include <tuple>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Mesh.hpp"
#include "Types.hpp"

namespace Kuma3D {

/**
 * A struct containing all the information needed to
 * render a 2D glyph/character.
 *
 * All values are in pixels except for mAdvance, which is
 * measured in 1/64th pixels.
 */
struct GlyphInfo
{
  unsigned int mWidth    { 0 };  // the width of the glyph
  unsigned int mHeight   { 0 };  // the height of the glyph
  unsigned int mBearingX { 0 };  // the horizontal distance from the origin
  unsigned int mBearingY { 0 };  // the vertical distance from the origin
  unsigned int mAdvance  { 0 };  // the distance between this glyph's origin and the next

  unsigned int mOffsetX  { 0 };  // the x-position of the character in the font atlas
};

/**
 * Identifies a specific font face by a family/style pairing.
 */
struct FontFaceIdentifier
{
  std::string mFamily;
  std::string mStyle;

  bool operator<(const FontFaceIdentifier& rhs) const
  {
    return std::tie(mFamily, mStyle) < std::tie(rhs.mFamily, rhs.mStyle);
  };
};

/**
 * Identifies a font atlas using a font face/size pairing.
 */
struct FontTextureIdentifier
{
  FontFaceIdentifier mFace;
  unsigned int mSize { 12 };

  bool operator<(const FontTextureIdentifier& rhs) const
  {
    return std::tie(mSize, mFace) < std::tie(rhs.mSize, rhs.mFace);
  };
};

/**
 * A static class that handles loading fonts and creating OpenGL textures for them.
 */
class FontLoader
{
  public:

    /**
     * Loads a font from the file at the given path. If the font at
     * the given path has already been loaded, this function does nothing.
     *
     * @param aFilePath The path to the font file.
     */
    static void LoadFontFromFile(const std::string& aFilePath);

    /**
     * Creates an OpenGL texture containing each glyph supported by the given
     * font at the given size and returns the texture ID. If such a texture has
     * already been created, this function simply returns the existing ID. If
     * the specified font hasn't been loaded yet, this function fails.
     *
     * @param aFontTextureID The FontTextureIdentifier to create a texture for.
     * @return The ID the stored OpenGL texture.
     */
    static ID LoadTextureForFont(const FontTextureIdentifier& aFontTextureID);

    /**
     * Unloads each loaded font, and destroys all font textures as well.
     * If you load any fonts, be sure to call this before the program
     * terminates to avoid memory leaks.
     */
    static void UnloadFontsAndTextures();

    /**
     * Updates a given Mesh to display the given text. Any vertices, indices,
     * and/or references to OpenGL textures already on the Mesh will be lost.
     *
     * @param aMesh The Mesh to change.
     * @param aText The text to display.
     * @param aFontTextureID The FontTextureIdentifier to display the font in.
     */
    static void UpdateMeshToDisplayText(Mesh& aMesh,
                                        const std::string& aText,
                                        const FontTextureIdentifier& aFontTextureID);

  private:

    /**
     * Creates and stores a GlyphMap for a given font at a given size.
     *
     * @param aFontTextureID The font to create a GlyphMap for.
     */
    static void CreateGlyphMapForFont(const FontTextureIdentifier& aFontTextureID);

    static bool mInitialized;
    static FT_Library mLibrary;

    // Maps single characters to the dimensions required to render them.
    using GlyphMap = std::map<char, GlyphInfo>;

    // Maps file paths to their family/style combinations.
    using FontFileMap = std::map<std::string, FontFaceIdentifier>;

    // Maps font family/style combinations to their font face object.
    using FontFaceMap = std::map<FontFaceIdentifier, FT_Face>;

    // Maps font/size combinations to their glyph data.
    using FontGlyphMap = std::map<FontTextureIdentifier, GlyphMap>;

    // Maps font/size combinations to their texture atlas.
    using FontTextureMap = std::map<FontTextureIdentifier, ID>;

    static FontFileMap mFontFileMap;
    static FontFaceMap mFontFaceMap;
    static FontGlyphMap mFontGlyphMap;
    static FontTextureMap mFontTextureMap;

    static const unsigned int mTexturePadding = 1;
};

} // namespace Kuma3D

#endif
