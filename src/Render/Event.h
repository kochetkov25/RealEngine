#pragma once

#include <functional>
#include <map>
#include <string>

#include "../Modules/Logger.h"

namespace Render {

// Base class for all events
class Event {
public:
  enum class EventType {
    MOUSE_MOVED = 1,
    WINDOW_CLOSED,
    KEY_PRESSED,
    KEY_RELEASED,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    COUNT
  };

  virtual ~Event() = default;

  [[nodiscard]] std::string getName() const { return _name; }
  [[nodiscard]] EventType getType() const { return _type; }

  // Format event as string for logging/debugging
  virtual std::string format() const = 0;

protected:
  EventType _type;
  std::string _name;

  Event(std::string name, EventType type)
      : _type(type), _name(std::move(name)) {}
};

// Dispatches events to registered listeners
class EventDispatcher {
public:
  template <typename TEvent>
  void addEventListener(std::function<void(TEvent &)> callback) {
    // Wrapper that converts base Event to specific TEvent
    auto base = [func = std::move(callback)](Event &e) {
      func(static_cast<TEvent &>(e));
    };
    // Create temporary event to get its type
    TEvent tempEvent;
    _eventCallbacks.emplace(tempEvent.getType(), base);
  }

  // Dispatch event to registered listeners
  void dispatch(Event &event) {
    const auto it = _eventCallbacks.find(event.getType());
    if (it != _eventCallbacks.end()) {
      it->second(event);
    } else {
      Core::Logger::debug("EventDispatcher",
                          "Event not registered: ", event.getName());
    }
  }

private:
  // Map of event types to their callback functions
  std::map<Event::EventType, std::function<void(Event &)>> _eventCallbacks;
};

} // namespace Render
