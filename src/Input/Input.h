#pragma once

#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"

namespace Core {
class Input {
 public:
  static bool isKeyPressed(const int key);

  static void releaseKey(const int key);

  static void pressKey(const int key);

  static void pressMouseBtn(const int key);

  static void releaseMouseBtn(const int key);

  static bool isMouseBtnPressed(const int key);

  static void setMousePosition(const glm::vec2& vec);

  static glm::vec2 getMousePosition();

 private:
  static bool _keys[static_cast<size_t>(GLFW_KEY_LAST)];

  static bool _mouseBtns[static_cast<size_t>(GLFW_MOUSE_BUTTON_LAST)];

  static glm::vec2 _mousePos;
};
}  // namespace Core