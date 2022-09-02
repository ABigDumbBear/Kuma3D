#include <Game.hpp>
#include <WindowOptions.hpp>

#include <AudioLoader.hpp>
#include <ShaderLoader.hpp>
#include <TextureLoader.hpp>

#include "SceneFactory.hpp"

int main()
{
  // Initialize the engine.
  Kuma3D::WindowOptions options;
  options.mTitle = "KumaTetris";
  options.mWidth = 320;
  options.mHeight = 800;

  Kuma3D::Game::Initialize(options);

  // Load the shaders.
  Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/GridShader.vert",
                                            "resources/shaders/GridShader.frag");
  Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/TileShader.vert",
                                            "resources/shaders/TileShader.frag");

  // Load the textures.
  Kuma3D::TextureLoader::LoadTextureFromFile("resources/tileTexture.png");

  // Load the audio.
  Kuma3D::AudioLoader::LoadAudioFromFile("resources/BGM.wav");

  // Create a Game and load a Scene.
  Kuma3D::Game game;
  game.SetScene(KumaTetris::CreateScene());
  game.Run();

  // Clean up.
  Kuma3D::AudioLoader::UnloadAudio();
  Kuma3D::TextureLoader::UnloadTextures();
  Kuma3D::ShaderLoader::UnloadShaders();
  Kuma3D::Game::Uninitialize();

  return 0;
}
