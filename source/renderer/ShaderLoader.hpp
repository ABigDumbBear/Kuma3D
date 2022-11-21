#ifndef SHADERLOADER_HPP
#define SHADERLOADER_HPP

#include <map>
#include <string>

#include <GL/glew.h>

#include "Vec3.hpp"
#include "Mat4.hpp"

#include "IDGenerator.hpp"

namespace Kuma3D {

using VertexFragmentPair = std::pair<std::string, std::string>;

/**
 * A static class that handles loading and manipulating GLSL shaders.
 */
class ShaderLoader
{
  public:

    /**
     * Loads the vertex and fragment files at the given locations, then
     * compiles the shader. If a shader program with the given files has
     * already been created, this function does nothing but return the ID
     * of the corresponding shader program.
     *
     * @param aVertexFile The path to a vertex shader.
     * @param aFragmentFile The path to a fragment shader.
     * @return The ID of the shader program in OpenGL.
     */
    static ID LoadShaderFromFiles(const std::string& aVertexFile,
                                  const std::string& aFragmentFile);

    /**
     * Loads the vertex and fragment source text, then compiles the shader.
     *
     * @param aVertexSource The source text of a vertex shader.
     * @param aFragmentSource The source text of a fragment shader.
     */
    static ID LoadShaderFromText(const std::string& aVertexSource,
                                 const std::string& aFragmentSource);

    /**
     * Unloads all OpenGL shader programs. If you load any shaders, be sure
     * to call this before the program terminates to avoid memory leaks.
     */
    static void UnloadShaders();

    /**
     * Returns whether a uniform exists in the given shader.
     *
     * @param aID The ID of the shader to check.
     * @param aName The name of the uniform to check.
     * @return True if the uniform exists, false otherwise.
     */
    static bool IsUniformDefined(const ID& aID,
                                 const std::string& aName);

    /**
     * Sets an integer uniform on this shader.
     *
     * @param aID The ID of the shader to change.
     * @param aName The name of the uniform to set.
     * @param aValue The value to set the uniform to.
     */
    static void SetInt(const ID& aID,
                       const std::string& aName,
                       int aValue);

    /**
     * Sets a float uniform on this shader.
     *
     * @param aID The ID of the shader to change.
     * @param aName The name of the uniform to set.
     * @param aValue The value to set the uniform to.
     */
    static void SetFloat(const ID& aID,
                         const std::string& aName,
                         float aValue);

    /**
     * Sets a Vec3 uniform on this shader.
     *
     * @param aID The ID of the shader to change.
     * @param aName The name of the uniform to set.
     * @param aValue The value to set the uniform to.
     */
    static void SetVec3(const ID& aID,
                        const std::string& aName,
                        const Vec3& aValue);

    /**
     * Sets a Mat4 uniform on this shader.
     *
     * @param aID The ID of the shader to change.
     * @param aName The name of the uniform to set.
     * @param aValue The value to set the uniform to.
     */
    static void SetMat4(const ID& aID,
                        const std::string& aName,
                        const Mat4& aValue);

  private:

    /**
     * Compiles a GLSL shader and assigns an ID to it.
     *
     * @param aShaderID The ID to assign to this shader.
     * @param aShaderSource The source code of the shader.
     * @param aShaderType The type of the shader (GL_VERTEX_SHADER,
     *                    GL_FRAGMENT_SHADER, etc.)
     */
    static void CompileShader(ID& aShaderID,
                              const std::string& aShaderSource,
                              GLenum aShaderType);

    /**
     * Creates a shader program given the IDs to a vertex shader
     * and a fragment shader. This is done after both shaders have
     * successfully compiled.
     *
     * @param aVertexID The ID of the vertex shader as known to OpenGL.
     * @param aFragmentID The ID of the fragment shader as known to OpenGL.
     * @return The shader program ID used by OpenGL.
     */
    static ID CreateProgram(const ID& aVertexID,
                            const ID& aFragmentID);

    static std::map<VertexFragmentPair, ID> mShaderMap;
};

} // namespace Kuma3D

#endif
