#pragma once

#include <stdexcept>
#include <string>

namespace Resources {

/**
 * @brief Base exception class for model loading errors
 */
class ModelLoadException : public std::runtime_error {
public:
  explicit ModelLoadException(const std::string &message)
      : std::runtime_error("ModelLoadException: " + message) {}
  virtual ~ModelLoadException() = default;
};

/**
 * @brief Exception thrown when a model file cannot be found or opened
 */
class ModelFileNotFoundException : public ModelLoadException {
public:
  explicit ModelFileNotFoundException(const std::string &filePath)
      : ModelLoadException("Model file not found: " + filePath),
        filePath_(filePath) {}
  const std::string &getFilePath() const noexcept { return filePath_; }

private:
  std::string filePath_;
};

/**
 * @brief Exception thrown when Assimp fails to import a model
 */
class ModelImportException : public ModelLoadException {
public:
  explicit ModelImportException(const std::string &filePath,
                                const std::string &assimpError)
      : ModelLoadException("Failed to import model: " + filePath +
                           ". Error: " + assimpError),
        filePath_(filePath), assimpError_(assimpError) {}
  const std::string &getFilePath() const noexcept { return filePath_; }
  const std::string &getAssimpError() const noexcept { return assimpError_; }

private:
  std::string filePath_;
  std::string assimpError_;
};

/**
 * @brief Exception thrown when a model file is corrupted or invalid
 */
class ModelCorruptedException : public ModelLoadException {
public:
  explicit ModelCorruptedException(const std::string &filePath,
                                   const std::string &reason)
      : ModelLoadException("Corrupted model file: " + filePath +
                           ". Reason: " + reason),
        filePath_(filePath), reason_(reason) {}
  const std::string &getFilePath() const noexcept { return filePath_; }
  const std::string &getReason() const noexcept { return reason_; }

private:
  std::string filePath_;
  std::string reason_;
};

/**
 * @brief Exception thrown when texture loading fails
 */
class TextureLoadException : public ModelLoadException {
public:
  explicit TextureLoadException(const std::string &textureName,
                                const std::string &reason)
      : ModelLoadException("Failed to load texture: " + textureName +
                           ". Reason: " + reason),
        textureName_(textureName), reason_(reason) {}
  const std::string &getTextureName() const noexcept { return textureName_; }
  const std::string &getReason() const noexcept { return reason_; }

private:
  std::string textureName_;
  std::string reason_;
};

} // namespace Resources
