#include <Game.hpp>
#include <ShaderLoader.hpp>
#include <WindowOptions.hpp>

#include "SceneFactory.hpp"

int main()
{
  // Initialize the engine.
  Kuma3D::WindowOptions options;
  options.mTitle = "Tetris";
  options.mWidth = 320;
  options.mHeight = 800;

  Kuma3D::Game::Initialize(options);

  // Load the shaders.
  Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/GridShader.vert",
                                            "resources/shaders/GridShader.frag");

  // Create a Game and load a Scene.
  Kuma3D::Game game;
  game.SetScene(Tetris::CreateScene());
  game.Run();

  // Clean up.
  Kuma3D::ShaderLoader::UnloadShaders();
  Kuma3D::Game::Uninitialize();

  return 0;
}
