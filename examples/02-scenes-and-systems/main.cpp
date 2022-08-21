#include <Game.hpp>

#include <FontLoader.hpp>
#include <ShaderLoader.hpp>
#include <TextureLoader.hpp>

#include "SceneFactory.hpp"

int main()
{
  // Initialize the engine.
  Kuma3D::WindowOptions options;
  options.mTitle = "Scenes and Systems";
  options.mWidth =1200;
  options.mHeight = 720;

  Kuma3D::Game::Initialize(options);

  // Load the cube texture.
  auto cubeTextureID = Kuma3D::TextureLoader::LoadTextureFromFile("resources/template.png");

  // Load the cube shader.
  auto cubeShaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/Default.vert",
                                                                      "resources/shaders/Default.frag");

  // Create a Game and set the scene.
  Kuma3D::Game game;
  game.SetScene(example_2::CreateScene(cubeTextureID, cubeShaderID));
  game.Run();

  // Remember to clean up!
  Kuma3D::TextureLoader::UnloadTextures();
  Kuma3D::ShaderLoader::UnloadShaders();
  Kuma3D::FontLoader::UnloadFontsAndTextures();
  Kuma3D::Game::Uninitialize();

  return 0;
}
