#include "Window.h"

#include <iostream>

#include "../Input/Input.h"
#include "../Modules/GUIModule.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

namespace Render {

Render::Window::Window() {
  _init = false;
  _pWindow = nullptr;
  _width = 1024;
  _height = 768;
  _windowName = "mainWindow";

  setFunCallBack([this](Event& e) { this->_dispatcher.dispatch(e); });
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

bool Window::windowShouldClose() { return glfwWindowShouldClose(_pWindow); }

bool Render::Window::init_GLFW() {
  if (!glfwInit()) {
    std::cout << "Error load GLFW!" << std::endl;
    return false;
  }
  return true;
}

bool Render::Window::init_GLAD() {
  if (!gladLoadGL()) {
    std::cout << "Error load GLAD!" << std::endl;
    return false;
  } else {
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor
              << std::endl;
  }
  return true;
}

void Render::Window::initEvents() {
  _dispatcher.addEventListner<MouseMovedEvent>([](MouseMovedEvent& e) {
    Core::Input::setMousePosition(e.getPosition());
  });

  _dispatcher.addEventListner<MouseButtonPressed>([](MouseButtonPressed& e) {
    std::cout << e.format() << std::endl;
    Core::Input::pressMouseBtn(e.getButton());
  });

  _dispatcher.addEventListner<MouseButtonReleased>([](MouseButtonReleased& e) {
    std::cout << e.format() << std::endl;
    Core::Input::releaseMouseBtn(e.getButton());
  });

  _dispatcher.addEventListner<WindowClosedEvent>(
      [](WindowClosedEvent& e) { std::cout << e.format() << std::endl; });

  _dispatcher.addEventListner<KeyPressedEvent>([](KeyPressedEvent& e) {
    std::cout << e.format() << std::endl;
    Core::Input::pressKey(e.getKey());
  });

  _dispatcher.addEventListner<KeyReleasedEvent>([](KeyReleasedEvent& e) {
    std::cout << e.format() << std::endl;
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
    _pWindow =
        glfwCreateWindow(_width, _height, _windowName.c_str(), NULL, NULL);
    if (!_pWindow) {
      std::cout << "Error create window: " << _windowName << std::endl;
      glfwTerminate();
      return false;
    }
  }

  glfwMakeContextCurrent(_pWindow);

  initEvents();
  glfwSetWindowUserPointer(_pWindow, this);
  glfwSetCursorPosCallback(_pWindow, mouseMovedCallBack);
  glfwSetWindowCloseCallback(_pWindow, windowClosedCallBack);
  glfwSetKeyCallback(_pWindow, keyCallBack);
  glfwSetMouseButtonCallback(_pWindow, mouseButtonCallBack);

  if (!init_GLAD()) return false;

  std::cout << "Window Inited!" << std::endl;
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

  glClearColor(0 / 255.f, 0 / 255.f, 0 / 255.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::mouseMovedCallBack(GLFWwindow* pWindow, double x, double y) {
  auto& handle = *static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
  MouseMovedEvent event(x, y);
  handle.funCallBack(event);
}

void Render::Window::windowClosedCallBack(GLFWwindow* pWindow) {
  auto& handle = *static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
  WindowClosedEvent event;
  handle.funCallBack(event);
}

void Render::Window::keyCallBack(GLFWwindow* pWindow, int key, int scancode,
                                 int action, int mods) {
  auto& handle = *static_cast<Window*>(glfwGetWindowUserPointer(pWindow));

  switch (action) {
    case GLFW_PRESS: {
      KeyPressedEvent eventPressed(key);
      handle.funCallBack(eventPressed);
      break;
    }
    case GLFW_RELEASE: {
      KeyReleasedEvent eventReleased(key);
      handle.funCallBack(eventReleased);
      break;
    }
    default: {
      break;
    }
  }
}

void Window::mouseButtonCallBack(GLFWwindow* pWindow, int button, int action,
                                 int mods) {
  auto& handle = *static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
  if (action == GLFW_PRESS) {
    MouseButtonPressed eventMousePressed(button);
    handle.funCallBack(eventMousePressed);
  } else {
    MouseButtonReleased eventMouseReleased(button);
    handle.funCallBack(eventMouseReleased);
  }
}
}  // namespace Render