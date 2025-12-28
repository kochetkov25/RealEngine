#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include "Render/ShaderUtils/UniformCamera.h"
#include "Render/UniformBuffer.h"

namespace Render {

class Camera {
 public:
  // Projection mode type
  enum class ProjectionMode {
    PERSPECTIVE,
    ORTHOGRAPHIC_CENTER,
    ORTHOGRAPHIC_LEFT_BOT
  };

  // Constructor
  // Note: For correct projection matrix calculation,
  // the window size must be set after construction.
  // The constructor initializes the frustum.
  // windowHeight and windowWidth should be set
  // in the correct order for proper frustum calculation.
  Camera(const glm::vec3& position = {0, 0, 0},
         const glm::vec3& rotation = {0, 0, -90}, const float nearPlane = 0.1f,
         const float farplane = 100.f, const float windowHeight = 768.f,
         const float windowWidth = 1024.f,
         const ProjectionMode projMode = ProjectionMode::PERSPECTIVE,
         const float velocity = 0.0f, const float sensitivity = 0.05f)
      : _position(position),
        _rotation(rotation),
        _projectionMode(projMode),
        _nearPlane(nearPlane),
        _farPlane(farplane),
        _windowHeight(windowHeight),
        _windowWidth(windowWidth),
        _velocity(velocity),
        _sensitivity(sensitivity),
        _cameraUBO(0) {
    updateViewMat();
    updateProjMat();
  }

  // Set near and far plane
  void setPlane(const float near, const float far);
  // Set window size for projection calculations
  void setWindowSize(const float height, const float width);

  // Set camera position and update view matrix
  void setPosition(const glm::vec3& position);
  // Set camera rotation
  void setRotation(const glm::vec3& rotation);

  // Set both position and rotation simultaneously
  void setPositionRotation(const glm::vec3& position,
                           const glm::vec3& rotation);
  // Set projection mode
  void setProjectionMode(ProjectionMode mode);

  // Get view matrix
  [[nodiscard]] glm::mat4 getViewMat() const;
  // Get projection matrix
  [[nodiscard]] glm::mat4 getProjMat() const;

  void setVelocity(const float velocity) { _velocity = velocity; }
  void setSensitivity(const float sensitivity);

  void moveCamera(const float duration);

  [[nodiscard]] glm::vec3 getPosition() const { return _position; }

  [[nodiscard]] ShaderUtils::CameraBlock getCameraBlock() const;

  void update();

 private:
  ProjectionMode _projectionMode;

  glm::vec3 _position;
  glm::vec3 _rotation;  // ROLL PITCH YAW
  glm::vec3 _front = {0.0f, 0.0f, -1.0f};
  glm::vec3 _up = {0.0f, 1.0f, 0.0f};

  bool _initMouse = true;
  glm::vec2 _initialMousePos = {0.0f, 0.0f};

  glm::mat4 _viewMat;
  glm::mat4 _projMat;

  float _nearPlane;
  float _farPlane;

  float _windowHeight;
  float _windowWidth;

  float _velocity;
  float _sensitivity;

  UniformBuffer<ShaderUtils::CameraBlock> _cameraUBO;

  // Update view matrix
  void updateViewMat();
  // Update projection matrix
  void updateProjMat();

  void rotateCamera();
};

}  // namespace Render
