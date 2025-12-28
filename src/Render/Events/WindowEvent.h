#pragma once

#include "../Event.h"

namespace Render {

class WindowClosedEvent : public Event {
 public:
  WindowClosedEvent()
      : Event("Window Closed Event", Event::EventType::WINDOW_CLOSED) {}

  [[nodiscard]] std::string format() const override {
    return _name + "!";
  }
};

}  // namespace Render
