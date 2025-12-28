#pragma once

#include "../Event.h"
#include <glm/vec2.hpp>

namespace Render {

// Event fired when mouse moves
class MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(int x = 0, int y = 0)
      : Event("MOUSE MOVED EVENT", EventType::MOUSE_MOVED), _x(x), _y(y) {}

  [[nodiscard]] std::string format() const override {
    return _name + ": X = " + std::to_string(_x) + ", Y = " + std::to_string(_y);
  }

  [[nodiscard]] glm::vec2 getPosition() const { return glm::vec2(_x, _y); }

 private:
  int _x{0};
  int _y{0};
};

// Event fired when mouse button is pressed
class MouseButtonPressed : public Event {
 public:
  explicit MouseButtonPressed(int button = 0)
      : Event("MOUSE BUTTON PRESSED", EventType::MOUSE_BUTTON_PRESSED),
        _button(button) {}

  [[nodiscard]] std::string format() const override {
    return _name + ": " + std::to_string(_button);
  }

  [[nodiscard]] int getButton() const { return _button; }

 private:
  int _button{0};
};

// Event fired when mouse button is released
class MouseButtonReleased : public Event {
 public:
  explicit MouseButtonReleased(int button = 0)
      : Event("MOUSE BUTTON RELEASED", EventType::MOUSE_BUTTON_RELEASED),
        _button(button) {}

  [[nodiscard]] std::string format() const override {
    return _name + ": " + std::to_string(_button);
  }

  [[nodiscard]] int getButton() const { return _button; }

 private:
  int _button{0};
};

}  // namespace Render
