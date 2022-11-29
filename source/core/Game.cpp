#include "Game.hpp"

#include <algorithm>
#include <iostream>

#include <stdexcept>

#include "GameSignals.hpp"
#include "InputSignals.hpp"

namespace Kuma3D {

bool Game::mInitialized = false;
GLFWwindow* Game::mWindow = nullptr;

std::map<int, std::vector<GamepadButton>> Game::mGamepadButtonMap;

std::unique_ptr<Scene> Game::mScene = nullptr;
std::unique_ptr<Scene> Game::mNewScene = nullptr;

bool Game::mExiting = false;

/*****************************************************************************/
void GLFWErrorCallback(int aError, const char* aDescription)
{
  std::cout << "GLFW encountered an error: " << aDescription << std::endl;
}

/*****************************************************************************/
void GLFWFramebufferSizeCallback(GLFWwindow* aWindow, int aWidth, int aHeight)
{
  //glViewport(0, 0, aWidth, aHeight);
}

/*****************************************************************************/
void GLFWKeyPressedCallback(GLFWwindow* aWindow,
                            int aKey,
                            int aScancode,
                            int aAction,
                            int aMods)
{
  auto action = static_cast<InputAction>(aAction);
  KeyCode keyCode = static_cast<KeyCode>(aKey);
  switch(action)
  {
    case InputAction::ePRESSED:
    {
      KeyPressed.Notify(keyCode, aMods);
      break;
    }
    case InputAction::eRELEASED:
    {
      KeyReleased.Notify(keyCode, aMods);
      break;
    }
    case InputAction::eREPEATED:
    {
      KeyRepeated.Notify(keyCode, aMods);
      break;
    }
  }
}

/*****************************************************************************/
void GLFWMouseMovedCallback(GLFWwindow* aWindow, double aX, double aY)
{
  MouseMoved.Notify(aX, aY);
}

/*****************************************************************************/
void GLFWMouseEnteredOrLeftCallback(GLFWwindow* aWindow, int aEntered)
{
  auto entered = static_cast<bool>(aEntered);
  MouseEnteredOrLeft.Notify(entered);
}

/*****************************************************************************/
void GLFWMouseButtonPressedCallback(GLFWwindow* aWindow,
                                    int aButton,
                                    int aAction,
                                    int aMods)
{
  auto action = static_cast<InputAction>(aAction);
  switch(action)
  {
    case InputAction::ePRESSED:
    {
      MouseButtonPressed.Notify(static_cast<MouseButton>(aButton), aMods);
      break;
    }
    case InputAction::eRELEASED:
    {
      MouseButtonReleased.Notify(static_cast<MouseButton>(aButton), aMods);
      break;
    }
    case InputAction::eREPEATED:
    {
      break;
    }
  }
}

/*****************************************************************************/
void GLFWMouseScrolledCallback(GLFWwindow* aWindow,
                               double aXOffset,
                               double aYOffset)
{
  MouseScrolled.Notify(aXOffset, aYOffset);
}

/******************************************************************************/
void Game::Initialize(const WindowOptions& aOptions)
{
  // Before initializing, set up the GLFW error callback.
  glfwSetErrorCallback(GLFWErrorCallback);

  // Initialize GLFW.
  mInitialized = glfwInit();
  if(mInitialized)
  {
    // Set the desired OpenGL version to 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Use the core profile only; this removes backwards-compatible features
    // that are no longer needed for the engine.
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);

    // Enable forward compatibility; this removes all deprecated features
    // in the desired version of OpenGL (3.3).
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GLFW_TRUE);

    // Enable double buffering.
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // Create a new window.
    CreateWindow(aOptions);

    // Initialize GLEW.
    auto glewError = glewInit();
    if(glewError != GLEW_OK)
    {
      std::cout << "Error initializing GLEW! "
                << glewGetErrorString(glewError) << std::endl;
      mInitialized = false;
    }
  }
}

/******************************************************************************/
void Game::Uninitialize()
{
  // Clean up GLFW.
  glfwDestroyWindow(mWindow);
  glfwTerminate();

  mInitialized = false;
  mWindow = nullptr;
}

/******************************************************************************/
void Game::ApplyOptionsToWindow(const WindowOptions& aOptions)
{
  switch(aOptions.mWindowMode)
  {
    case WindowMode::eWINDOWED:
    {
      glfwSetWindowMonitor(mWindow,
                           nullptr,
                           0,
                           0,
                           aOptions.mWidth,
                           aOptions.mHeight,
                           GLFW_DONT_CARE);
      break;
    }
    case WindowMode::eFULLSCREEN:
    {
      glfwSetWindowMonitor(mWindow,
                           glfwGetPrimaryMonitor(),
                           0,
                           0,
                           aOptions.mWidth,
                           aOptions.mHeight,
                           GLFW_DONT_CARE);
      break;
    }
    case WindowMode::eBORDERLESS_FULLSCREEN:
    {
      const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
      glfwSetWindowMonitor(mWindow,
                           glfwGetPrimaryMonitor(),
                           0,
                           0,
                           mode->width,
                           mode->height,
                           mode->refreshRate);
      break;
    }
  }

  // Set the GLFW cursor input mode.
  auto cursorModeAsInt = static_cast<int>(aOptions.mCursorMode);
  glfwSetInputMode(mWindow, GLFW_CURSOR, cursorModeAsInt);

  // Set the window title.
  glfwSetWindowTitle(mWindow, aOptions.mTitle.c_str());
}

/******************************************************************************/
void Game::Run()
{
  if(!mInitialized)
  {
    throw std::logic_error("The Game must be initialized before calling Run()!");
  }
  else if(mScene == nullptr)
  {
    throw std::logic_error("The Game must have a Scene before calling Run()!");
  }

  // Run until the window is closed or until Exit() is called.
  while(!glfwWindowShouldClose(mWindow) &&
        !mExiting)
  {
    if(mNewScene != nullptr)
    {
      mScene.reset(nullptr);
      mScene = std::move(mNewScene);
      mNewScene.reset(nullptr);
    }

    // Process the GLFW event queue.
    glfwPollEvents();

    // GLFW doesn't use events for gamepad/gamepad input, so do that here.
    PollGamepadButtons();

    // Update the current Scene.
    mScene->OperateSystems(glfwGetTime());

    // Swap the front/back buffers.
    glfwSwapBuffers(mWindow);
  }

  GamePendingExit.Notify(glfwGetTime());
  mScene.reset(nullptr);
  mExiting = false;
}

/******************************************************************************/
void Game::Exit()
{
  mExiting = true;
}

/******************************************************************************/
void Game::SetScene(std::unique_ptr<Scene> aScene)
{
  if(mScene == nullptr)
  {
    mScene = std::move(aScene);
  }
  else
  {
    mNewScene = std::move(aScene);
  }
}

/******************************************************************************/
float Game::GetGamepadAxisValue(int aID, const GamepadAxis& aAxis)
{
  float value = 0;

  GLFWgamepadstate state;
  if(glfwGetGamepadState(aID, &state))
  {
    value = state.axes[static_cast<int>(aAxis)];
  }

  return value;
}

/******************************************************************************/
void Game::CreateWindow(const WindowOptions& aOptions)
{
  // Destroy the current window.
  if(mWindow != nullptr)
  {
    glfwDestroyWindow(mWindow);
  }

  // Create a new window.
  mWindow = glfwCreateWindow(1, 1, "", nullptr, nullptr);

  // Connect GLFW input callback functions.
  glfwSetFramebufferSizeCallback(mWindow, GLFWFramebufferSizeCallback);
  glfwSetKeyCallback(mWindow, GLFWKeyPressedCallback);
  glfwSetCursorPosCallback(mWindow, GLFWMouseMovedCallback);
  glfwSetCursorEnterCallback(mWindow, GLFWMouseEnteredOrLeftCallback);
  glfwSetMouseButtonCallback(mWindow, GLFWMouseButtonPressedCallback);
  glfwSetScrollCallback(mWindow, GLFWMouseScrolledCallback);
  glfwSetJoystickCallback(HandleGamepadEvent);

  // Create the OpenGL context.
  glfwMakeContextCurrent(mWindow);

  // Apply the set options to the window.
  ApplyOptionsToWindow(aOptions);
}

/*****************************************************************************/
void Game::PollGamepadButtons()
{
  GLFWgamepadstate state;

  for(auto& gamepadButtonPair : mGamepadButtonMap)
  {
    if(glfwGetGamepadState(gamepadButtonPair.first, &state))
    {
      // For each button, determine if it was pressed or released this frame.
      for(int i = 0; i < 15; ++i)
      {
        // Find the button in the gamepad's button list.
        auto button = static_cast<GamepadButton>(i);
        auto foundButton = std::find(gamepadButtonPair.second.begin(),
                                     gamepadButtonPair.second.end(),
                                     button);

        if(state.buttons[i])
        {
          // The button is pressed, so check if it's already in the button list.
          // If it isn't, put it there and notify the ButtonPressed signal.
          if(foundButton == gamepadButtonPair.second.end())
          {
            gamepadButtonPair.second.emplace_back(button);
            ButtonPressed.Notify(gamepadButtonPair.first, button);
          }
        }
        else
        {
          // The button is not pressed, so check if it's in the button list.
          // If it is, remove it and notify the ButtonReleased signal.
          if(foundButton != gamepadButtonPair.second.end())
          {
            gamepadButtonPair.second.erase(foundButton);
            ButtonReleased.Notify(gamepadButtonPair.first, button);
          }
        }
      }
    }
  }
}

/*****************************************************************************/
void Game::HandleGamepadEvent(int aID, int aEvent)
{
  auto foundGamepad = mGamepadButtonMap.find(aID);

  if(aEvent == GLFW_CONNECTED && foundGamepad == mGamepadButtonMap.end())
  {
    std::vector<GamepadButton> buttons;
    mGamepadButtonMap.emplace(aID, buttons);
    GamepadConnected.Notify(aID);
  }
  else if(aEvent == GLFW_DISCONNECTED && foundGamepad != mGamepadButtonMap.end())
  {
    mGamepadButtonMap.erase(aID);
    GamepadDisconnected.Notify(aID);
  }
}

} // namespace Kuma3D
