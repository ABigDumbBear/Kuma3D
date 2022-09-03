#include <Game.hpp>
#include <WindowOptions.hpp>

#include <ShaderLoader.hpp>

#include "SceneFactory.hpp"

int main()
{
  // Initialize the engine.
  Kuma3D::WindowOptions options;
  options.mTitle = "Untitled";
  options.mWidth = 1280;
  options.mHeight = 720;

  Kuma3D::Game::Initialize(options);

  // Load the shaders.
  Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/ShipShader.vert",
                                            "resources/shaders/ShipShader.frag");

  // Create a Game and load a Scene.
  Kuma3D::Game game;
  game.SetScene(Untitled::CreateScene());
  game.Run();

  // Clean up.
  Kuma3D::ShaderLoader::UnloadShaders();
  Kuma3D::Game::Uninitialize();

  return 0;
}
