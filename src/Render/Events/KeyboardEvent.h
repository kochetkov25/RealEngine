#pragma once

#include "../Event.h"

namespace Render {

// Event fired when a key is pressed
class KeyPressedEvent : public Event {
 public:
  explicit KeyPressedEvent(int key = 0)
      : Event("KEY PRESSED EVENT", Event::EventType::KEY_PRESSED), _key(key) {}

  [[nodiscard]] std::string format() const override {
    return _name + ": " + std::to_string(_key);
  }

  [[nodiscard]] int getKey() const { return _key; }

 private:
  int _key{0};
};

// Event fired when a key is released
class KeyReleasedEvent : public Event {
 public:
  explicit KeyReleasedEvent(int key = 0)
      : Event("KEY RELEASED EVENT", Event::EventType::KEY_RELEASED),
        _key(key) {}

  [[nodiscard]] std::string format() const override {
    return _name + ": " + std::to_string(_key);
  }

  [[nodiscard]] int getKey() const { return _key; }

 private:
  int _key{0};
};

}  // namespace Render
