#include "Texture2D.h"

namespace Render {

// Creates a texture and uploads pixel data to OpenGL.
// In the current implementation, texture units are not used,
// so textures are bound directly to the active texture unit
Texture2D::Texture2D(const GLuint width, const GLuint height,
                     const unsigned char *textureData,
                     const unsigned int channels, const GLenum filter,
                     const GLenum wrapMode)
    : _height(height), _width(width) {
  // Determine the number of color channels
  GLenum internalFormat;
  switch (channels) {
  case 4:
    _mode = GL_RGBA;
    internalFormat = GL_RGBA8;
    break;
  case 3:
    _mode = GL_RGB;
    internalFormat = GL_RGB8;
    break;
  case 2:
    _mode = GL_RG;
    internalFormat = GL_RG8;
    break;
  case 1:
    _mode = GL_RED;
    internalFormat = GL_R8;
    break;
  default:
    _mode = GL_RGBA;
    internalFormat = GL_RGBA8;
    break;
  }
  // Generate texture ID
  glGenTextures(1, &_ID);
  /*
   * In OpenGL, there are up to 16 texture units for textures.
   * For now, textures are bound directly to the active texture unit
   */
  // glActiveTexture(GL_TEXTURE0);
  /*
   * Bind texture to target (2D texture)
   * (Note: if not bound, texture will not be created in 2D format)
   */
  glBindTexture(GL_TEXTURE_2D, _ID);
  // Upload pixel data to GPU
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, _mode,
               GL_UNSIGNED_BYTE, textureData);
  // Set texture parameters for texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
  // Set texture parameters for filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  // Generate mipmap levels
  glGenerateMipmap(GL_TEXTURE_2D);
  // Unbind texture from target (to avoid accidental modifications)
  glBindTexture(GL_TEXTURE_2D, 0);
}

Render::Texture2D::Texture2D(const GLuint width, const GLuint height,
                             const float *textureData,
                             const unsigned int channels, const GLenum filter,
                             const GLenum wrapMode)
    : _height(height), _width(width) {
  // Determine the number of color channels
  GLenum internalFormat;
  switch (channels) {
  case 4:
    _mode = GL_RGBA;
    internalFormat = GL_RGBA32F;
    break;
  case 3:
    _mode = GL_RGB;
    internalFormat = GL_RGB32F;
    break;
  case 2:
    _mode = GL_RG;
    internalFormat = GL_RG32F;
    break;
  case 1:
    _mode = GL_RED;
    internalFormat = GL_R32F;
    break;
  default:
    _mode = GL_RGBA;
    internalFormat = GL_RGBA32F;
    break;
  }
  // Generate texture ID
  glGenTextures(1, &_ID);
  /*
   * In OpenGL, there are up to 16 texture units for textures.
   * For now, textures are bound directly to the active texture unit
   */
  // glActiveTexture(GL_TEXTURE0);
  /*
   * Bind texture to target (2D texture)
   * (Note: if not bound, texture will not be created in 2D format)
   */
  glBindTexture(GL_TEXTURE_2D, _ID);
  // Upload pixel data to GPU (using float format)
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, _mode,
               GL_FLOAT, textureData);
  // Set texture parameters for texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
  // Set texture parameters for filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  // Generate mipmap levels
  glGenerateMipmap(GL_TEXTURE_2D);
  // Unbind texture from target (to avoid accidental modifications)
  glBindTexture(GL_TEXTURE_2D, 0);
}

// Destructor: deletes the OpenGL texture
Texture2D::~Texture2D() { glDeleteTextures(1, &_ID); }

// Bind the texture to the specified texture unit
void Texture2D::bindTexture2D(const unsigned short texBlock) {
  glActiveTexture(GL_TEXTURE0 + texBlock);
  glBindTexture(GL_TEXTURE_2D, _ID);
}

// Add a sub-texture to the texture atlas
void Texture2D::addSubTexture2D(std::string textureName,
                                const glm::vec2 &leftBottomUV,
                                const glm::vec2 &rightTopUV) {
  _subTextures2Dmap.emplace(textureName,
                            subTexture2D(leftBottomUV, rightTopUV));
}

/*
 * Retrieves a sub-texture from the texture atlas.
 * If the texture is not found, returns
 * the default sub-texture coordinates
 */
Texture2D::subTexture2D &
Texture2D::getSubTexture2D(const std::string &textureName) {
  auto it = _subTextures2Dmap.find(textureName);
  if (it != _subTextures2Dmap.end()) {
    return it->second;
  }
  // If texture is not found, return default texture coordinates
  static subTexture2D defTexture2D;
  return defTexture2D;
}

} // namespace Render
