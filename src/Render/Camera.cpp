#include "Camera.h"

#include <algorithm>

#include "../Input/Input.h"

namespace Render {
// Set near and far clipping planes
void Camera::setPlane(const float near, const float far) {
  _nearPlane = near;
  _farPlane = far;
  updateProjMat();
}

// Set window size for projection matrix calculation
void Camera::setWindowSize(const float height, const float width) {
  _windowHeight = height;
  _windowWidth = width;
  updateProjMat();
}

// Set camera position
void Camera::setPosition(const glm::vec3& position) {
  _position = position;
  updateViewMat();
}

// Set camera rotation
void Camera::setRotation(const glm::vec3& rotation) {
  _rotation = rotation;
  rotateCamera();
  updateViewMat();
}

// Set both position and rotation
void Camera::setPositionRotation(const glm::vec3& position,
                                 const glm::vec3& rotation) {
  _position = position;
  _rotation = rotation;
  rotateCamera();
  updateViewMat();
}

// Set projection mode
void Camera::setProjectionMode(ProjectionMode mode) {
  _projectionMode = mode;
  updateProjMat();
}

// Get view matrix
glm::mat4 Camera::getViewMat() const { return _viewMat; }

// Get projection matrix
glm::mat4 Camera::getProjMat() const { return _projMat; }

// Update the view matrix
void Camera::updateViewMat() {
  _viewMat = glm::lookAt(_position, _position + _front, _up);
}

// Update the projection matrix
void Camera::updateProjMat() {
  const float aspect = _windowWidth / _windowHeight;
  switch (_projectionMode) {
    case Render::Camera::ProjectionMode::PERSPECTIVE:
      _projMat =
          glm::perspective(glm::radians(45.f), aspect, _nearPlane, _farPlane);
      break;
    case Render::Camera::ProjectionMode::ORTHOGRAPHIC_CENTER:
      _projMat = glm::ortho(-_windowWidth / 2.f, _windowWidth / 2.f,
                            -_windowHeight / 2.f, _windowHeight / 2.f,
                            _nearPlane, _farPlane);
      break;
    case Render::Camera::ProjectionMode::ORTHOGRAPHIC_LEFT_BOT:
      _projMat = glm::ortho(0.f, _windowWidth, 0.f, _windowHeight, _nearPlane,
                            _farPlane);
      break;
    default:
      _projMat = glm::perspective(glm::radians(45.f), aspect, _nearPlane, _farPlane);
      break;
  }
}

void Camera::moveCamera(const float duration) {
  bool isUpdateViewMat = false;
  if (Core::Input::isKeyPressed(GLFW_KEY_W)) {
    _position += (duration * _velocity) * _front;
    isUpdateViewMat = true;
  }

  if (Core::Input::isKeyPressed(GLFW_KEY_S)) {
    _position -= (duration * _velocity) * _front;
    isUpdateViewMat = true;
  }

  if (Core::Input::isKeyPressed(GLFW_KEY_A)) {
    _position -=
        glm::normalize(glm::cross(_front, _up)) * (duration * _velocity);
    isUpdateViewMat = true;
  }

  if (Core::Input::isKeyPressed(GLFW_KEY_D)) {
    _position +=
        glm::normalize(glm::cross(_front, _up)) * (duration * _velocity);
    isUpdateViewMat = true;
  }

  if (Core::Input::isKeyPressed(GLFW_KEY_SPACE)) {
    _position += _up * (duration * _velocity);
    isUpdateViewMat = true;
  }

  if (Core::Input::isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
    _position -= _up * (duration * _velocity);
    isUpdateViewMat = true;
  }

  if (Core::Input::isMouseBtnPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
    auto currentMousePos = Core::Input::getMousePosition();

    if (_initMouse) {
      _initialMousePos.x = currentMousePos.x;
      _initialMousePos.y = currentMousePos.y;
      _initMouse = false;
    }

    _rotation.z -= (_initialMousePos.x - currentMousePos.x) * _sensitivity;
    _rotation.y += (_initialMousePos.y - currentMousePos.y) * _sensitivity;

    _initialMousePos.x = currentMousePos.x;
    _initialMousePos.y = currentMousePos.y;

    rotateCamera();

    isUpdateViewMat = true;
  } else {
    _initMouse = true;
  }

  if (isUpdateViewMat) updateViewMat();
}

ShaderUtils::CameraBlock Camera::getCameraBlock() const {
  return {getViewMat(), getProjMat(), getPosition()};
}

void Camera::update() { _cameraUBO.set(getCameraBlock()); }

void Camera::rotateCamera() {
  _front.x = cos(glm::radians(_rotation[1])) * cos(glm::radians(_rotation[2]));
  _front.y = sin(glm::radians(_rotation[1]));
  _front.z = cos(glm::radians(_rotation[1])) * sin(glm::radians(_rotation[2]));
}

void Camera::setSensitivity(const float sensitivity) {
  _sensitivity = (sensitivity >= 1) ? 1 : sensitivity;
}
}  // namespace Render
