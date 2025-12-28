// clang-format off

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <string>

#include "Event.h"

// clang-format on

namespace Render {
class Event;
class EventDispatcher;

class Window {
 public:
  enum class RenderMode { Fill, Wireframe, Point };

  Window();
  ~Window();

  // Deleted copy and move constructors/assignments
  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(const Window&) = delete;
  Window& operator=(Window&&) = delete;

  // Update window state (swap buffers, poll events, clear)
  void update();
  // Initialize window and OpenGL context
  bool init();

  void initEvents();

  // Getters for window dimensions
  [[nodiscard]] unsigned int getWidth() const { return _width; }
  [[nodiscard]] unsigned int getHeight() const { return _height; }
  [[nodiscard]] GLFWwindow* getWindow() const { return _pWindow; }

  // Setters for window properties
  void setName(std::string name) { _windowName = std::move(name); }
  void setResolution(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
  }

  void setRenderMode(RenderMode mode);

  [[nodiscard]] bool windowShouldClose() const;

  // Event dispatcher for window events
  EventDispatcher _dispatcher;

 private:
  std::string _windowName;  // Window title

  GLFWwindow* _pWindow;  // Pointer to GLFW window handle

  unsigned int _width{1024};
  unsigned int _height{768};

  bool init_GLFW();
  bool init_GLAD();

  bool _init{false};  // Initialization flag

  // Callback function for event handling
  std::function<void(Event&)> _eventCallback;

  // Set the event callback function
  void setEventCallback(const std::function<void(Event&)>& callback) {
    _eventCallback = callback;
  }

  // Static callback functions for GLFW
  static void mouseMovedCallback(GLFWwindow* window, double x, double y);
  static void windowClosedCallback(GLFWwindow* window);
  static void keyCallback(GLFWwindow* window, int key, int scancode,
                          int action, int mods);
  static void mouseButtonCallback(GLFWwindow* window, int button, int action,
                                  int mods);
};
}  // namespace Render
