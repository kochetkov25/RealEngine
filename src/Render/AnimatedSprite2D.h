#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Render {
class ShaderProgram;
class Texture2D;

class AnimatedSprite2D {
 public:
  struct FrameData {
    std::string subTextureName;
    std::chrono::nanoseconds duration;
    glm::vec2 uvMin;
    glm::vec2 uvMax;

    FrameData(const std::string &name, std::chrono::nanoseconds dur, const glm::vec2 &minUV, const glm::vec2 &maxUV)
        : subTextureName(name), duration(dur), uvMin(minUV), uvMax(maxUV) {}
  };

  AnimatedSprite2D(std::shared_ptr<Texture2D> pTexture2D, std::shared_ptr<ShaderProgram> pShaderProgram,
                   const glm::vec3 &position = glm::vec3(0.0f), const glm::vec2 &size = glm::vec2(100.0f),
                   float rotation = 0.0f);

  ~AnimatedSprite2D() = default;

  AnimatedSprite2D(const AnimatedSprite2D &) = delete;
  AnimatedSprite2D &operator=(const AnimatedSprite2D &) = delete;
  AnimatedSprite2D(AnimatedSprite2D &&) noexcept = default;
  AnimatedSprite2D &operator=(AnimatedSprite2D &&) noexcept = default;

  void setAnimationParameters(const std::vector<std::pair<std::string, std::chrono::nanoseconds>> &framesDurations);

  void update(std::chrono::nanoseconds deltaTime);
  void update(float deltaTimeSeconds);
  void render() const;

  void setPosition(const glm::vec3 &position) { _position = position; }
  void setSize(const glm::vec2 &size) { _size = size; }
  void setRotation(float rotation, const glm::vec3 &axis = glm::vec3(0.0f, 0.0f, 1.0f)) {
    _rotation = rotation;
    _rotationAxis = axis;
  }

  [[nodiscard]] const glm::vec3 &getPosition() const { return _position; }
  [[nodiscard]] const glm::vec2 &getSize() const { return _size; }
  [[nodiscard]] float getRotation() const { return _rotation; }
  [[nodiscard]] size_t getCurrentFrame() const { return _currentFrame; }
  [[nodiscard]] size_t getFrameCount() const { return _frames.size(); }

  void resetAnimation();
  void setPlaybackSpeed(float speed) { _playbackSpeed = std::max(0.0f, speed); }

 private:
  [[nodiscard]] glm::mat4 buildModelMatrix() const;
  void updateCurrentFrame();

  std::shared_ptr<Texture2D> _pTexture2D;
  std::shared_ptr<ShaderProgram> _pShaderProgram;

  glm::vec3 _position;
  glm::vec2 _size;
  float _rotation;
  glm::vec3 _rotationAxis;

  std::vector<FrameData> _frames;
  size_t _currentFrame;
  std::chrono::nanoseconds _currentFrameTime;
  float _playbackSpeed;

  mutable std::shared_ptr<class Renderer> _pRenderer;
  mutable bool _rendererInitialized;

  void initializeRenderer() const;
};

}  // namespace Render