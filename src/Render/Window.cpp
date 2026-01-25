#include "Window.h"

#include <cassert>

#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"
#include "Input/Input.h"
#include "Modules/GUIModule.h"
#include "Modules/Logger.h"


namespace Render {

Render::Window::Window() : _windowName("mainWindow"), _pWindow(nullptr) {
  setEventCallback([this](Event &e) { this->_dispatcher.dispatch(e); });
}

void Window::setRenderMode(RenderMode mode) {
  switch (mode) {
    case RenderMode::Fill:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case RenderMode::Wireframe:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case RenderMode::Point:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      break;
    default:
      assert(false && "Unknown RenderMode!");
  }
}

bool Window::windowShouldClose() const { return glfwWindowShouldClose(_pWindow); }

bool Render::Window::init_GLFW() {
  if (!glfwInit()) {
    Core::Logger::error("Window", "Failed to initialize GLFW!");
    return false;
  }
  return true;
}

bool Render::Window::init_GLAD() {
  if (!gladLoadGL()) {
    Core::Logger::error("Window", "Failed to initialize GLAD!");
    return false;
  } else {
    Core::Logger::info("Window", "Renderer: ", glGetString(GL_RENDERER));
    Core::Logger::info("Window", "OpenGL: ", GLVersion.major, ".", GLVersion.minor);
  }
  return true;
}

void Render::Window::initEvents() {
  _dispatcher.addEventListener<MouseMovedEvent>(
      [](MouseMovedEvent &e) { Core::Input::setMousePosition(e.getPosition()); });

  _dispatcher.addEventListener<MouseButtonPressed>([](MouseButtonPressed &e) {
    Core::Logger::debug("Window", e.format());
    Core::Input::pressMouseBtn(e.getButton());
  });

  _dispatcher.addEventListener<MouseButtonReleased>([](MouseButtonReleased &e) {
    Core::Logger::debug("Window", e.format());
    Core::Input::releaseMouseBtn(e.getButton());
  });

  _dispatcher.addEventListener<WindowClosedEvent>(
      [](WindowClosedEvent &e) { Core::Logger::debug("Window", e.format()); });

  _dispatcher.addEventListener<KeyPressedEvent>([](KeyPressedEvent &e) {
    Core::Logger::debug("Window", e.format());
    Core::Input::pressKey(e.getKey());
  });

  _dispatcher.addEventListener<KeyReleasedEvent>([](KeyReleasedEvent &e) {
    Core::Logger::debug("Window", e.format());
    Core::Input::releaseKey(e.getKey());
  });
}

static bool GLFWinit = false;
bool Render::Window::init() {
  if (_init) {
    return true;
  }

  if (!init_GLFW()) {
    return false;
  }
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  /*enable 8x MSAA*/
  glfwWindowHint(GLFW_SAMPLES, 8);

  if (!_pWindow) {
    _pWindow = glfwCreateWindow(_width, _height, _windowName.c_str(), NULL, NULL);
    if (!_pWindow) {
      Core::Logger::error("Window", "Failed to create window: ", _windowName);
      glfwTerminate();
      return false;
    }
  }

  glfwMakeContextCurrent(_pWindow);

  initEvents();
  glfwSetWindowUserPointer(_pWindow, this);
  glfwSetCursorPosCallback(_pWindow, mouseMovedCallback);
  glfwSetWindowCloseCallback(_pWindow, windowClosedCallback);
  glfwSetKeyCallback(_pWindow, keyCallback);
  glfwSetMouseButtonCallback(_pWindow, mouseButtonCallback);

  if (!init_GLAD()) return false;

  Core::Logger::info("Window", "Window initialized successfully!");
  _init = true;
  return true;
}

Render::Window::~Window() {
  Modules::GUIModule::onWindowClose();
  glfwDestroyWindow(_pWindow);
  glfwTerminate();
}

void Render::Window::update() {
  glfwSwapBuffers(_pWindow);
  glfwPollEvents();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::mouseMovedCallback(GLFWwindow *window, double x, double y) {
  auto &handle = *static_cast<Window *>(glfwGetWindowUserPointer(window));
  MouseMovedEvent event(static_cast<int>(x), static_cast<int>(y));
  handle._eventCallback(event);
}

void Render::Window::windowClosedCallback(GLFWwindow *window) {
  auto &handle = *static_cast<Window *>(glfwGetWindowUserPointer(window));
  WindowClosedEvent event;
  handle._eventCallback(event);
}

void Render::Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  (void)scancode;  // Unused parameter
  (void)mods;      // Unused parameter

  auto &handle = *static_cast<Window *>(glfwGetWindowUserPointer(window));

  switch (action) {
    case GLFW_PRESS: {
      KeyPressedEvent eventPressed(key);
      handle._eventCallback(eventPressed);
      break;
    }
    case GLFW_RELEASE: {
      KeyReleasedEvent eventReleased(key);
      handle._eventCallback(eventReleased);
      break;
    }
    default:
      break;
  }
}

void Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  (void)mods;  // Unused parameter

  auto &handle = *static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (action == GLFW_PRESS) {
    MouseButtonPressed eventMousePressed(button);
    handle._eventCallback(eventMousePressed);
  } else if (action == GLFW_RELEASE) {
    MouseButtonReleased eventMouseReleased(button);
    handle._eventCallback(eventMouseReleased);
  }
}
}  // namespace Render