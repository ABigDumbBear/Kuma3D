#ifndef WINDOWOPTIONS_HPP
#define WINDOWOPTIONS_HPP

#include <string>

namespace Kuma3D {

/**
 * An enumeration for the three supported types of windows.
 */
enum class WindowMode
{
  eWINDOWED,
  eFULLSCREEN,
  eBORDERLESS_FULLSCREEN
};

/**
 * An enumeration for each supported cursor mode. These are assigned
 * the same values as the corresponding modes in GLFW to allow for
 * compatibility after casting to a static_cast to int.
 */
enum class CursorMode
{
  eNORMAL = 0x00034001,
  eHIDDEN = 0x00034002,
  eDISABLED = 0x00034003
};

/**
 * Contains several options used during window creation.
 */
struct WindowOptions
{
  WindowMode mWindowMode { WindowMode::eWINDOWED };
  CursorMode mCursorMode { CursorMode::eNORMAL };

  int mWidth { 1200 };
  int mHeight { 800 };

  std::string mTitle;
};

} // namespace Kuma3D

#endif
