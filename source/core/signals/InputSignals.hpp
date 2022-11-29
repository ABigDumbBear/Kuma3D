#ifndef INPUTSIGNALS_HPP
#define INPUTSIGNALS_HPP

#include <string>

#include "Signal.hpp"

namespace Kuma3D {

/**
 * An enumeration for each supported GLFW key code, defined
 * in the same order as GLFW and assigned the same value.
 */
enum class KeyCode
{
  eKEY_UNKNOWN = -1,
  eKEY_SPACE = 32,
  eKEY_APOSTROPHE = 39,
  eKEY_COMMA = 44,
  eKEY_MINUS = 45,
  eKEY_PERIOD = 46,
  eKEY_SLASH = 47,
  eKEY_0 = 48,
  eKEY_1 = 49,
  eKEY_2 = 50,
  eKEY_3 = 51,
  eKEY_4 = 52,
  eKEY_5 = 53,
  eKEY_6 = 54,
  eKEY_7 = 55,
  eKEY_8 = 56,
  eKEY_9 = 57,
  eKEY_SEMICOLON = 59,
  eKEY_EQUAL = 61,
  eKEY_A = 65,
  eKEY_B = 66,
  eKEY_C = 67,
  eKEY_D = 68,
  eKEY_E = 69,
  eKEY_F = 70,
  eKEY_G = 71,
  eKEY_H = 72,
  eKEY_I = 73,
  eKEY_J = 74,
  eKEY_K = 75,
  eKEY_L = 76,
  eKEY_M = 77,
  eKEY_N = 78,
  eKEY_O = 79,
  eKEY_P = 80,
  eKEY_Q = 81,
  eKEY_R = 82,
  eKEY_S = 83,
  eKEY_T = 84,
  eKEY_U = 85,
  eKEY_V = 86,
  eKEY_W = 87,
  eKEY_X = 88,
  eKEY_Y = 89,
  eKEY_Z = 90,
  eKEY_LEFT_BRACKET = 91,
  eKEY_BACKSLASH = 92,
  eKEY_RIGHT_BRACKET = 93,
  eKEY_GRAVE_ACCENT = 96,
  eKEY_WORLD1 = 161,
  eKEY_WORLD2 = 162,
  eKEY_ESCAPE = 256,
  eKEY_ENTER = 257,
  eKEY_TAB = 258,
  eKEY_BACKSPACE = 259,
  eKEY_INSERT = 260,
  eKEY_DELETE = 261,
  eKEY_RIGHT = 262,
  eKEY_LEFT = 263,
  eKEY_DOWN = 264,
  eKEY_UP = 265,
  eKEY_PAGE_UP = 266,
  eKEY_PAGE_DOWN = 267,
  eKEY_HOME = 268,
  eKEY_END = 269,
  eKEY_CAPS_LOCK = 280,
  eKEY_SCROLL_LOCK = 281,
  eKEY_NUM_LOCK = 282,
  eKEY_PRINT_SCREEN = 283,
  eKEY_PAUSE = 284,
  eKEY_F1 = 290,
  eKEY_F2 = 291,
  eKEY_F3 = 292,
  eKEY_F4 = 293,
  eKEY_F5 = 294,
  eKEY_F6 = 295,
  eKEY_F7 = 296,
  eKEY_F8 = 297,
  eKEY_F9 = 298,
  eKEY_F10 = 299,
  eKEY_F11 = 300,
  eKEY_F12 = 301,
  eKEY_F13 = 302,
  eKEY_F14 = 303,
  eKEY_F15 = 304,
  eKEY_F16 = 305,
  eKEY_F17 = 306,
  eKEY_F18 = 307,
  eKEY_F19 = 308,
  eKEY_F20 = 309,
  eKEY_F21 = 310,
  eKEY_F22 = 311,
  eKEY_F23 = 312,
  eKEY_F24 = 313,
  eKEY_F25 = 314,
  eKEY_KP_0 = 320,
  eKEY_KP_1 = 321,
  eKEY_KP_2 = 322,
  eKEY_KP_3 = 323,
  eKEY_KP_4 = 324,
  eKEY_KP_5 = 325,
  eKEY_KP_6 = 326,
  eKEY_KP_7 = 327,
  eKEY_KP_8 = 328,
  eKEY_KP_9 = 329,
  eKEY_KP_DECIMAL = 330,
  eKEY_KP_DIVIDE = 331,
  eKEY_KP_MULTIPLY = 332,
  eKEY_KP_SUBTRACT = 333,
  eKEY_KP_ADD = 334,
  eKEY_KP_ENTER = 335,
  eKEY_KP_EQUAL = 336,
  eKEY_LEFT_SHIFT = 340,
  eKEY_LEFT_CONTROL = 341,
  eKEY_LEFT_ALT = 342,
  eKEY_LEFT_SUPER = 343,
  eKEY_RIGHT_SHIFT = 344,
  eKEY_RIGHT_CONTROL = 345,
  eKEY_RIGHT_ALT = 346,
  eKEY_RIGHT_SUPER = 347,
  eKEY_MENU = 348
};

/**
 * An enumeration for each supported GLFW key/button action, defined
 * in the same order as GLFW and assigned the same value.
 */
enum class InputAction
{
  eRELEASED = 0,
  ePRESSED = 1,
  eREPEATED = 2
};

/**
 * An enumeration for each supported GLFW keyboard modifier, defined
 * in the same order as GLFW and assigned the same value.
 */
enum class KeyboardModifier
{
  eMOD_SHIFT = 0x0001,
  eMOD_CONTROL = 0X0002,
  eMOD_ALT = 0x0004,
  eMOD_SUPER = 0x0008,
  eMOD_CAPS_LOCK = 0x0010,
  eMOD_NUM_LOCK = 0x0020
};

/**
 * An enumeration for each supported GLFW mouse button, defined
 * in the same order as GLFW and assigned the same value.
 */
enum class MouseButton
{
  eMOUSE_BUTTON_1 = 0,
  eMOUSE_BUTTON_2 = 1,
  eMOUSE_BUTTON_3 = 2,
  eMOUSE_BUTTON_4 = 3,
  eMOUSE_BUTTON_5 = 4,
  eMOUSE_BUTTON_6 = 5,
  eMOUSE_BUTTON_7 = 6,
  eMOUSE_BUTTON_8 = 7,
  eMOUSE_BUTTON_LEFT = eMOUSE_BUTTON_1,
  eMOUSE_BUTTON_RIGHT = eMOUSE_BUTTON_2,
  eMOUSE_BUTTON_MIDDLE = eMOUSE_BUTTON_3
};

/**
 * An enumeration for each supported GLFW gamepad button, defined
 * in the same order as GLFW and assigned the same value.
 */
enum class GamepadButton
{
  eGAMEPAD_BUTTON_A = 0,
  eGAMEPAD_BUTTON_B = 1,
  eGAMEPAD_BUTTON_X = 2,
  eGAMEPAD_BUTTON_Y = 3,
  eGAMEPAD_BUTTON_LEFT_BUMPER = 4,
  eGAMEPAD_BUTTON_RIGHT_BUMPER = 5,
  eGAMEPAD_BUTTON_BACK = 6,
  eGAMEPAD_BUTTON_START = 7,
  eGAMEPAD_BUTTON_GUIDE = 8,
  eGAMEPAD_BUTTON_LEFT_THUMB = 9,
  eGAMEPAD_BUTTON_RIGHT_THUMB = 10,
  eGAMEPAD_BUTTON_DPAD_UP = 11,
  eGAMEPAD_BUTTON_DPAD_RIGHT = 12,
  eGAMEPAD_BUTTON_DOWN = 13,
  eGAMEPAD_BUTTON_LEFT = 14
};

/**
 * An enumeration for each supported GLFW gamepad joystick axis, defined
 * in the same order as GLFW and assigned the same value.
 */
enum class GamepadAxis
{
  eGAMEPAD_AXIS_LEFT_X = 0,
  eGAMEPAD_AXIS_LEFT_Y = 1,
  eGAMEPAD_AXIS_RIGHT_X = 2,
  eGAMEPAD_AXIS_RIGHT_Y = 3,
  eGAMEPAD_AXIS_LEFT_TRIGGER = 4,
  eGAMEPAD_AXIS_RIGHT_TRIGGER = 5
};

/**
 * Returns a string representing the given key.
 *
 * @param aCode The KeyCode of the key in question.
 * @return A string representation of the key.
 */
std::string GetKeyName(const KeyCode& aCode);

/**
 * Checks if a certain key modifier is active in the given GLFW
 * modifier bitmask.
 *
 * @param aMod The modifier to check.
 * @param aMods The bitmask that may or may not contain the modifier.
 * @return True if the given modifier is active, false otherwise.
 */
bool IsModifierActive(const KeyboardModifier& aMod, int aMods);

extern SignalT<KeyCode, int> KeyPressed;
extern SignalT<KeyCode, int> KeyReleased;
extern SignalT<KeyCode, int> KeyRepeated;

extern SignalT<double, double> MouseMoved;
extern SignalT<bool> MouseEnteredOrLeft;
extern SignalT<MouseButton, int> MouseButtonPressed;
extern SignalT<MouseButton, int> MouseButtonReleased;
extern SignalT<double, double> MouseScrolled;

extern SignalT<int> JoystickConnected;
extern SignalT<int> JoystickDisconnected;

extern SignalT<int, GamepadButton> ButtonPressed;
extern SignalT<int, GamepadButton> ButtonReleased;

} // namespace Kuma3D

#endif
