#include "ShaderLoader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace Kuma3D {

std::map<VertexFragmentPair, ID> ShaderLoader::mShaderMap;

/******************************************************************************/
ID ShaderLoader::LoadShaderFromFiles(const std::string& aVertexFile,
                                     const std::string& aFragmentFile)
{
  VertexFragmentPair pair(aVertexFile, aFragmentFile);
  auto foundShader = mShaderMap.find(pair);
  if(foundShader == mShaderMap.end())
  {
    std::ifstream vertexInput, fragmentInput;
    vertexInput.open(pair.first);
    fragmentInput.open(pair.second);

    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexInput.rdbuf();
    fragmentStream << fragmentInput.rdbuf();

    vertexInput.close();
    fragmentInput.close();

    auto shaderID = LoadShaderFromText(vertexStream.str(), fragmentStream.str());
    mShaderMap.emplace(pair, shaderID);
  }

  return mShaderMap[pair];
}

/******************************************************************************/
ID ShaderLoader::LoadShaderFromText(const std::string& aVertexSource,
                                    const std::string& aFragmentSource)
{
  // First, compile the shaders.
  ID vertexID, fragmentID;

  CompileShader(vertexID, aVertexSource, GL_VERTEX_SHADER);
  CompileShader(fragmentID, aFragmentSource, GL_FRAGMENT_SHADER);

  // Then, create and link the shader program.
  auto programID = CreateProgram(vertexID, fragmentID);

  // Finally, delete the shaders since they have been linked into the
  // program and are no longer needed.
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);

  return programID;
}

/******************************************************************************/
void ShaderLoader::UnloadShaders()
{
  for(const auto& shaderPair : mShaderMap)
  {
    glDeleteProgram(shaderPair.second);
  }

  mShaderMap.clear();
}

/******************************************************************************/
bool ShaderLoader::IsUniformDefined(const ID& aID,
                                    const std::string& aName)
{
  return glGetUniformLocation(aID, aName.c_str()) != -1;
}

/******************************************************************************/
void ShaderLoader::SetInt(const ID& aID, const std::string& aName, int aValue)
{
  int loc = glGetUniformLocation(aID, aName.c_str());
  if(loc != -1)
  {
    glUniform1i(loc, aValue);
  }
  else
  {
    std::cout << "Uniform " << aName << " does not exist "
              << "or is reserved!" << std::endl;
  }
}

/******************************************************************************/
void ShaderLoader::SetFloat(const ID& aID, const std::string& aName, float aValue)
{
  int loc = glGetUniformLocation(aID, aName.c_str());
  if(loc != -1)
  {
    glUniform1f(loc, aValue);
  }
  else
  {
    std::cout << "Uniform " << aName << " does not exist "
              << "or is reserved!" << std::endl;
  }
}

/******************************************************************************/
void ShaderLoader::SetVec3(const ID& aID, const std::string& aName, const Vec3& aVec)
{
  int loc = glGetUniformLocation(aID, aName.c_str());
  if(loc != -1)
  {
    glUniform3fv(loc, 1, &aVec.x);
  }
  else
  {
    std::cout << "Uniform " << aName << " does not exist "
              << "or is reserved!" << std::endl;
  }
}

/******************************************************************************/
void ShaderLoader::SetMat4(const ID& aID, const std::string& aName, const Mat4& aMat)
{
  int loc = glGetUniformLocation(aID, aName.c_str());
  if(loc != -1)
  {
    glUniformMatrix4fv(loc, 1, GL_FALSE, &aMat(0, 0));
  }
  else
  {
    std::cout << "Uniform " << aName << " does not exist "
              << "or is reserved!" << std::endl;
  }
}

/******************************************************************************/
void ShaderLoader::CompileShader(ID& aShaderID,
                                 const std::string& aShaderSource,
                                 GLenum aShaderType)
{
  const char* shaderCode = aShaderSource.c_str();

  // Error variables.
  int success;
  char infoLog[512];

  aShaderID = glCreateShader(aShaderType);
  glShaderSource(aShaderID, 1, &shaderCode, NULL);
  glCompileShader(aShaderID);

  // Check for compile errors.
  glGetShaderiv(aShaderID, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(aShaderID, 512, NULL, infoLog);
    std::cout << "Error compiling shader!\n" << infoLog << std::endl;
  }
}

/******************************************************************************/
ID ShaderLoader::CreateProgram(const ID& aVertexID,
                               const ID& aFragmentID)
{
  auto programID = glCreateProgram();
  glAttachShader(programID, aVertexID);
  glAttachShader(programID, aFragmentID);
  glLinkProgram(programID);

  // Error variables.
  int success;
  char infoLog[512];

  // Check for linking errors.
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(programID, 512, NULL, infoLog);
    std::cout << "Error linking program!\n" << infoLog << std::endl;
  }

  return programID;
}

} // namespace Kuma3D
