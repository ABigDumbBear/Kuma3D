#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.hpp"
#include "WindowOptions.hpp"

#include "InputSignals.hpp"

namespace Kuma3D {

/**
 * The Game class is responsible for the following:
 *
 * 1) Initializing third-party graphics libraries (GLFW and GLEW)
 * 2) Creating a window and an OpenGL context
 * 3) Connecting several callbacks to GLFW events (for input, resizing, etc).
 * 4) Updating a Scene
 */
class Game
{
  public:

    /**
     * Initializes third-party graphics libraries and creates a window
     * with an OpenGL context.
     *
     * @param aOptions The options to use during window creation.
     */
    static void Initialize(const WindowOptions& aOptions);

    /**
     * Uninitializes the third-party graphics libraries and destroys
     * the current window.
     */
    static void Uninitialize();

    /**
     * Applies new options to the window, if it exists.
     *
     * @param aOptions The options to apply to the window.
     */
    static void ApplyOptionsToWindow(const WindowOptions& aOptions);

    /**
     * Begins the game loop. This loop will run until Exit() is called
     * or until the window is closed.
     */
    static void Run();

    /**
     * Exits the game loop.
     */
    static void Exit();

    /**
     * Sets the Scene to update during the game loop.
     *
     * @param aScene The Scene to update during the game loop.
     */
    static void SetScene(std::unique_ptr<Scene> aScene);

  private:

    /**
     * Creates a window using the given options.
     */
    static void CreateWindow(const WindowOptions& aOptions);

    /**
     * Polls GLFW for the button state of each connected joystick and notifies
     * the ButtonPressed or ButtonReleased signal accordingly.
     */
    static void PollJoystickButtons();

    /**
     * Gets called whenever a joystick is connected or disconnected.
     *
     * @param aID The ID of the joystick.
     * @param aEvent Whether the joystick was connected or disconnected.
     */
    static void HandleJoystickEvent(int aID, int aEvent);

    static bool mInitialized;
    static GLFWwindow* mWindow;

    static std::map<int, std::vector<GamepadButton>> mJoystickButtonMap;

    static std::unique_ptr<Scene> mScene;
    static std::unique_ptr<Scene> mNewScene;

    static bool mExiting;
};

} // namespace Kuma3D

#endif
