#include <Game.hpp>
#include <Scene.hpp>

int main()
{
  Kuma3D::WindowOptions options;
  options.mTitle = "Hello World!";
  options.mWidth = 800;
  options.mHeight = 600;

  Kuma3D::Game::Initialize(options);

  Kuma3D::Game newGame;
  newGame.SetScene(std::make_unique<Kuma3D::Scene>());
  newGame.Run();

  Kuma3D::Game::Uninitialize();

  return 0;
}
