#include "AnimatedSprite2D.h"

#include "Modules/Logger.h"
#include "RendererFactory.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

namespace Render {

AnimatedSprite2D::AnimatedSprite2D(std::shared_ptr<Texture2D> pTexture2D, std::shared_ptr<ShaderProgram> pShaderProgram,
                                   const glm::vec3 &position, const glm::vec2 &size, float rotation)
    : _pTexture2D(std::move(pTexture2D)),
      _pShaderProgram(std::move(pShaderProgram)),
      _position(position),
      _size(size),
      _rotation(rotation),
      _rotationAxis(0.0f, 0.0f, 1.0f),
      _currentFrame(0),
      _currentFrameTime(std::chrono::nanoseconds(0)),
      _playbackSpeed(1.0f),
      _rendererInitialized(false) {
  if (!_pTexture2D) {
    Core::Logger::error("AnimatedSprite2D", "Texture2D is null");
  }
  if (!_pShaderProgram) {
    Core::Logger::error("AnimatedSprite2D", "ShaderProgram is null");
  }
}

void AnimatedSprite2D::setAnimationParameters(
    const std::vector<std::pair<std::string, std::chrono::nanoseconds>> &framesDurations) {
  if (!_pTexture2D) {
    Core::Logger::error("AnimatedSprite2D", "Cannot set animation parameters: texture is null");
    return;
  }

  _frames.clear();
  _frames.reserve(framesDurations.size());

  for (const auto &[name, duration] : framesDurations) {
    try {
      auto subTexture = _pTexture2D->getSubTexture2D(name);
      _frames.emplace_back(name, duration, subTexture._leftBottomUV, subTexture._rightTopUV);
    } catch (...) {
      Core::Logger::warning("AnimatedSprite2D", "Failed to get subtexture: ", name);
    }
  }

  if (_frames.empty()) {
    Core::Logger::warning("AnimatedSprite2D", "No valid frames added");
    return;
  }

  _currentFrame = 0;
  _currentFrameTime = std::chrono::nanoseconds(0);
}

void AnimatedSprite2D::update(std::chrono::nanoseconds deltaTime) {
  if (_frames.empty()) {
    return;
  }

  const auto adjustedDelta = std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime * _playbackSpeed);
  _currentFrameTime += adjustedDelta;
  updateCurrentFrame();
}

void AnimatedSprite2D::update(float deltaTimeSeconds) {
  update(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(deltaTimeSeconds)));
}

void AnimatedSprite2D::updateCurrentFrame() {
  if (_frames.empty()) {
    return;
  }

  const auto &currentFrameData = _frames[_currentFrame];
  const auto frameDuration = currentFrameData.duration;

  while (_currentFrameTime >= frameDuration && frameDuration.count() > 0) {
    _currentFrameTime -= frameDuration;
    _currentFrame = (_currentFrame + 1) % _frames.size();
  }
}

void AnimatedSprite2D::render() const {
  if (!_pShaderProgram || !_pTexture2D || _frames.empty()) {
    return;
  }

  if (!_rendererInitialized) {
    initializeRenderer();
  }

  if (!_pRenderer) {
    Core::Logger::error("AnimatedSprite2D", "Renderer not initialized");
    return;
  }

  _pShaderProgram->use();

  const auto modelMatrix = buildModelMatrix();
  _pShaderProgram->setMatrix4Uniform("modelMatrix", modelMatrix);

  const auto &frame = _frames[_currentFrame];
  _pShaderProgram->setVec2Uniform("spriteUVMin", frame.uvMin);
  _pShaderProgram->setVec2Uniform("spriteUVMax", frame.uvMax);

  _pTexture2D->bindTexture2D(0);
  _pShaderProgram->setTexUniform("spriteTexture", 0);
  _pShaderProgram->setBoolUniform("hasTexture", true);

  _pRenderer->drawArrays();
}

glm::mat4 AnimatedSprite2D::buildModelMatrix() const {
  glm::mat4 model(1.0f);
  model = glm::translate(model, _position);
  model = glm::translate(model, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));
  model = glm::rotate(model, glm::radians(_rotation), _rotationAxis);
  model = glm::translate(model, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));
  model = glm::scale(model, glm::vec3(_size, 1.0f));
  return model;
}

void AnimatedSprite2D::resetAnimation() {
  _currentFrame = 0;
  _currentFrameTime = std::chrono::nanoseconds(0);
}

void AnimatedSprite2D::initializeRenderer() const {
  _pRenderer = RendererFactory::CreateTextureRenderer();

  _pRenderer->vertex3(0.0f, 0.0f, 0.0f);
  _pRenderer->vertexUV(0.0f, 0.0f);
  _pRenderer->vertex3(1.0f, 0.0f, 0.0f);
  _pRenderer->vertexUV(1.0f, 0.0f);
  _pRenderer->vertex3(1.0f, 1.0f, 0.0f);
  _pRenderer->vertexUV(1.0f, 1.0f);

  _pRenderer->vertex3(1.0f, 1.0f, 0.0f);
  _pRenderer->vertexUV(1.0f, 1.0f);
  _pRenderer->vertex3(0.0f, 1.0f, 0.0f);
  _pRenderer->vertexUV(0.0f, 1.0f);
  _pRenderer->vertex3(0.0f, 0.0f, 0.0f);
  _pRenderer->vertexUV(0.0f, 0.0f);

  _pRenderer->upload();
  _rendererInitialized = true;
}

}  // namespace Render