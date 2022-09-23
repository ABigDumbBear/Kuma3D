#include "Game.hpp"

#include <iostream>

#include <stdexcept>

#include "GameSignals.hpp"
#include "InputSignals.hpp"

namespace Kuma3D {

bool Game::mInitialized = false;
GLFWwindow* Game::mWindow = nullptr;

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
Game::Game()
  : mScene(nullptr)
  , mExiting(false)
{
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
    glfwPollEvents();
    Update();

    // Swap the front/back buffers.
    glfwSwapBuffers(mWindow);
  }

  GamePendingExit.Notify(glfwGetTime());
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
  mScene = std::move(aScene);
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

  // Create the OpenGL context.
  glfwMakeContextCurrent(mWindow);

  // Apply the set options to the window.
  ApplyOptionsToWindow(aOptions);
}

/******************************************************************************/
void Game::Update()
{
  mScene->OperateSystems(glfwGetTime());
}

} // namespace Kuma3D
