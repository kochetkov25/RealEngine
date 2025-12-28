#include "Camera.h"

#include <algorithm>
#include <cmath>

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

// Set camera rotation (Euler angles in degrees: {roll, pitch, yaw})
// Converts to quaternion internally to avoid gimbal lock
void Camera::setRotation(const glm::vec3& rotation) {
  _pitch = std::clamp(rotation.y, kMinPitch, kMaxPitch);
  _yaw = rotation.z;
  updateOrientation();
  updateBasisVectors();
  updateViewMat();
}

// Set both position and rotation
void Camera::setPositionRotation(const glm::vec3& position,
                                 const glm::vec3& rotation) {
  _position = position;
  _pitch = std::clamp(rotation.y, kMinPitch, kMaxPitch);
  _yaw = rotation.z;
  updateOrientation();
  updateBasisVectors();
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

// Update the view matrix using quaternion-based orientation
void Camera::updateViewMat() {
  // Build view matrix from quaternion orientation
  // This approach is more stable than Euler angles and eliminates gimbal lock
  const glm::mat4 rotationMatrix = glm::mat4_cast(glm::conjugate(_orientation));
  const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), -_position);
  _viewMat = rotationMatrix * translationMatrix;
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

    // Calculate mouse delta
    // Horizontal movement (deltaX) should affect yaw (left/right rotation)
    // Vertical movement (deltaY) should affect pitch (up/down rotation)
    const float deltaX = (_initialMousePos.x - currentMousePos.x) * _sensitivity;
    const float deltaY = (_initialMousePos.y - currentMousePos.y) * _sensitivity;

    // Update yaw and pitch
    // Note: Both directions are inverted - moving mouse right rotates camera left, moving mouse up rotates camera down
    _yaw += deltaX;      // Horizontal mouse movement -> yaw rotation (inverted direction)
    _pitch += deltaY;    // Vertical mouse movement -> pitch rotation (inverted direction)
    
    // Clamp pitch to prevent flipping at ±90 degrees
    _pitch = std::clamp(_pitch, kMinPitch, kMaxPitch);

    // Update orientation quaternion and basis vectors
    updateOrientation();
    updateBasisVectors();

    _initialMousePos.x = currentMousePos.x;
    _initialMousePos.y = currentMousePos.y;

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

// Update orientation quaternion from yaw and pitch Euler angles
// This conversion is only for input handling; rotation is stored as quaternion
void Camera::updateOrientation() {
  const glm::quat yawQuat = glm::angleAxis(glm::radians(_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  const glm::quat pitchQuat = glm::angleAxis(glm::radians(_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
  
  _orientation = yawQuat * pitchQuat;
  _orientation = glm::normalize(_orientation);
}

// Update camera basis vectors (front, right, up) from orientation quaternion
void Camera::updateBasisVectors() {
  // Extract forward direction from quaternion
  // The forward vector in camera space is (0, 0, -1)
  const glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, -1.0f);
  _front = glm::normalize(_orientation * worldForward);
  
  // Extract right direction (1, 0, 0 in camera space)
  const glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
  _right = glm::normalize(_orientation * worldRight);
  
  // Extract up direction (0, 1, 0 in camera space)
  const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  _up = glm::normalize(_orientation * worldUp);
}

void Camera::setSensitivity(const float sensitivity) {
  _sensitivity = (sensitivity >= 1) ? 1 : sensitivity;
}
}  // namespace Render
