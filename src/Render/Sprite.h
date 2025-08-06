#pragma once
#include <memory>
#include <string>
#include <vector>

#include "VertexArray.h"
#include "glad/glad.h"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"

namespace Render {

class ShaderProgram;
class Texture2D;

class Sprite {
 public:
  struct FramePars {
    FramePars(const std::string name, const uint64_t duration)
        : _name(name), _duration(duration) {}
    std::string _name;
    uint64_t _duration;
  };

  Sprite(std::shared_ptr<Texture2D> pTexture2D,
         const std::string subTextureName,
         std::shared_ptr<ShaderProgram> pShaderProgram,
         const glm::vec3 &spritePosition, const glm::vec2 &spriteSize,
         const float rotation);

  ~Sprite();

  void renderSprite(short frameId = -1);

  void setSpritePosition(const glm::vec3 &spritePosition);

  void setSpriteSize(const glm::vec2 &spriteSize);

  void setSpriteRotation(const float rotationAng,
                         const glm::vec3 &axis = glm::vec3(0.f, 0.f, 1.f));

  uint64_t getFrameDuration(const size_t frameID) const;

  size_t getFramesCount() const;

  void setAnimParams(
      std::vector<std::pair<std::string, uint64_t>> &framesDurations);

 protected:
  std::shared_ptr<Texture2D> _pTexture2D;
  std::shared_ptr<ShaderProgram> _pShaderProgram;
  glm::vec3 _position;
  glm::vec2 _size;
  glm::vec3 _axis;
  float _rotation;

  std::shared_ptr<VertexBuffer> _pTexVertexVBO;
  VertexArray _VAO;

  std::vector<FramePars> _frameParams;

  short _lastFrameID;
};

}  // namespace Render