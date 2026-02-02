#include "FileManager.h"

#include <fstream>
#include <sstream>

#include "Modules/Logger.h"

namespace Resources {
void FileManager::initialize(const char* argv0) {
  if (argv0) {
    _rootPath = std::filesystem::absolute(argv0).parent_path();
  } else {
    _rootPath = std::filesystem::current_path();
  }
}

[[nodiscard]] std::string FileManager::readFile(const std::string& relativePath) const {
  if (!_rootPath) {
    Core::Logger::error("FileManager", "Root path not set. Cannot read file. Relative path: ", relativePath);
    return std::string();
  }

  std::filesystem::path fullPath = *_rootPath / relativePath;

  if (!std::filesystem::exists(fullPath)) {
    Core::Logger::error("FileManager", "File not found: ", fullPath.string());
    return std::string();
  }

  std::ifstream file(fullPath, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    Core::Logger::error("FileManager", "Could not open file: ", fullPath.string());
    return std::string();
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

[[nodiscard]] std::string FileManager::getAbsolutePath(const std::string& relativePath) const {
  if (!_rootPath) {
    Core::Logger::error("FileManager", "Root path not set. Cannot get absolute path. Relative path: ", relativePath);
    return std::string();
  }

  return (*_rootPath / relativePath).string();
}
}  // namespace Resources