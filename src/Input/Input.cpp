#include "../Input/Input.h"

namespace Core {
bool Input::_keys[static_cast<size_t>(GLFW_KEY_LAST)] = {};

bool Input::_mouseBtns[static_cast<size_t>(GLFW_MOUSE_BUTTON_LAST)] = {};

glm::vec2 Input::_mousePos = {};

bool Input::isKeyPressed(const int key) {
  return _keys[static_cast<size_t>(key)];
}

void Input::releaseKey(const int key) {
  _keys[static_cast<size_t>(key)] = false;
}

void Input::pressKey(const int key) { _keys[static_cast<size_t>(key)] = true; }

void Input::pressMouseBtn(const int key) {
  _mouseBtns[static_cast<size_t>(key)] = true;
}

void Input::releaseMouseBtn(const int key) {
  _mouseBtns[static_cast<size_t>(key)] = false;
}

bool Input::isMouseBtnPressed(const int key) {
  return _mouseBtns[static_cast<size_t>(key)];
}

void Input::setMousePosition(const glm::vec2& vec) { _mousePos = vec; }

glm::vec2 Core::Input::getMousePosition() { return _mousePos; }
}  // namespace Core