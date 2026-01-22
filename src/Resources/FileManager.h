#pragma once

#include <filesystem>
#include <optional>
namespace Resources {
class FileManager {
 public:
  FileManager(const FileManager&) = delete;
  FileManager& operator=(const FileManager&) = delete;

  static FileManager& instance() {
    static FileManager instance;
    return instance;
  }

  void initialize(const char* argv0);

  [[nodiscard]] std::string readFile(const std::string& relativePath) const;

  [[nodiscard]] std::string getAbsolutePath(const std::string& relativePath) const;

 private:
  FileManager() = default;

  std::optional<std::filesystem::path> _rootPath;
};
}  // namespace Resources