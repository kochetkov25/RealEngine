#include "ShaderManager.h"

#include <cassert>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>

#include "../Modules/Logger.h"
#include "../Render/ShaderProgram.h"
#include "FileManager.h"

namespace {
constexpr std::string_view kIncludeToken = "#include";
constexpr std::string_view kVersionToken = "#version";

const std::string kExtVertex = ".vert";
const std::string kExtFragment = ".frag";

const std::filesystem::path kShaderRoot = "res/shaders/";

constexpr std::string_view strTrim(std::string_view sv) {
  auto start = sv.find_first_not_of(" \t\n\r");
  if (start == std::string_view::npos) {
    return {};
  }

  auto end = sv.find_last_not_of(" \t\n\r");
  return sv.substr(start, end - start + 1);
}

std::string resolveRecursive(const std::string& currentSource, std::unordered_set<std::string>& includedFiles) {
  std::istringstream stream(currentSource);
  std::ostringstream output;
  std::string line;

  while (std::getline(stream, line)) {
    std::string_view lineView(line);
    std::string_view trimmed = strTrim(lineView);

    if (!trimmed.starts_with(kIncludeToken)) {
      output << line << '\n';
      continue;
    }

    auto filenamePart = strTrim(trimmed.substr(kIncludeToken.size()));
    if (filenamePart.empty() || !((filenamePart.front() == '"' && filenamePart.back() == '"') ||
                                  (filenamePart.front() == '<' && filenamePart.back() == '>'))) {
      Core::Logger::warning("ShaderManager", "Possible invalid include token in line: ", line);
      output << line << '\n';
      continue;
    }

    auto fileNameView = filenamePart.substr(1, filenamePart.size() - 2);
    std::string fileName(fileNameView);
    if (includedFiles.contains(fileName)) {
      continue;
    }
    includedFiles.insert(fileName);

    auto filePath = kShaderRoot / fileName;
    auto includeSource = Resources::FileManager::instance().readFile(filePath.string());
    if (includeSource.empty()) {
      Core::Logger::error("ShaderManager", "Failed to read or empty include: ", fileName);
      output << "// ERROR: Could not include " << fileName << "\n";
      continue;
    }

    output << resolveRecursive(includeSource, includedFiles) << '\n';
  }

  return output.str();
}

std::string resolveSahderIncludes(const std::string& shaderSource) {
  std::unordered_set<std::string> includedFiles;
  return resolveRecursive(shaderSource, includedFiles);
}

const std::unordered_map<Resources::ShaderType, std::string> kShaderNameMap = {
    {Resources::ShaderType::DebugGrid, "debug-grid"},
    {Resources::ShaderType::Light, "light"},
    {Resources::ShaderType::Sprite2D, "sprite2d"},
    {Resources::ShaderType::Mesh, "mesh"},
};

Resources::ShaderFeatures getSupportedFeatures(Resources::ShaderType type) {
  using namespace Resources;
  switch (type) {
    case ShaderType::Mesh:
      return ShaderFeatures::None | ShaderFeatures::Skinned;
    default:
      return ShaderFeatures::None;
  }
}

std::vector<std::string> getShaderDefines(Resources::ShaderFeatures features) {
  std::vector<std::string> defines;
  if (Resources::HasFlag(features, Resources::ShaderFeatures::Skinned)) {
    defines.push_back("SKINNED");
  }

  return defines;
}

std::string injectDefines(const std::string& sourceCode, const std::vector<std::string>& defines) {
  if (defines.empty()) {
    return sourceCode;
  }

  std::string definesCode;
  definesCode.reserve(defines.size() * 20);

  for (const auto& def : defines) {
    definesCode.append("#define ").append(def).append("\n");
  }

  std::string result;
  result.reserve(sourceCode.size() + definesCode.size() + 2);

  std::string_view sourceView(sourceCode);
  size_t versionPos = sourceView.find("#version");

  if (versionPos == std::string_view::npos) {
    result.append(definesCode);
    result.append(sourceView);
  } else {
    size_t lineEnd = sourceView.find('\n', versionPos);

    size_t splitPoint = (lineEnd == std::string_view::npos) ? sourceCode.size() : lineEnd + 1;

    result.append(sourceView.substr(0, splitPoint));

    result.append(definesCode);

    if (splitPoint < sourceCode.size()) {
      result.append(sourceView.substr(splitPoint));
    }
  }

  return result;
}
}  // namespace

namespace Resources {
[[nodiscard]] std::shared_ptr<Render::ShaderProgram> ShaderManager::getShaderProgram(const ShaderKey& key) {
  auto itShaderName = kShaderNameMap.find(key.type);
  if (itShaderName == kShaderNameMap.end()) {
    Core::Logger::error("ShaderManager", "Shader type not found (unknown shader name): ", static_cast<int>(key.type));
    assert(false);
    return nullptr;
  }

  auto supportedMask = getSupportedFeatures(key.type);
  auto supportedFeatures = key.features & supportedMask;
  if (supportedFeatures != key.features) {
    Core::Logger::warning("ShaderManager", "Requested features not supported for shader type: ", itShaderName->second,
                          ". They will be ignored.");
  }

  auto it = _shaderPrograms.find(ShaderKey{key.type, supportedFeatures});
  if (it != _shaderPrograms.end()) {
    return it->second;
  }

  return createShaderProgram(ShaderKey{key.type, supportedFeatures});
}

[[nodiscard]] std::shared_ptr<Render::ShaderProgram> ShaderManager::createShaderProgram(const ShaderKey& key) {
  std::string shaderName = kShaderNameMap.at(key.type);

  auto vertexShaderName = shaderName + kExtVertex;
  auto vertexShaderPathRelative = kShaderRoot / vertexShaderName;
  auto vertexShaderString = FileManager::instance().readFile(vertexShaderPathRelative.string());
  vertexShaderString = resolveSahderIncludes(vertexShaderString);
  vertexShaderString = injectDefines(vertexShaderString, getShaderDefines(key.features));
  if (vertexShaderString.empty()) {
    Core::Logger::error("ShaderManager", "Failed to load vertex shader: ", vertexShaderPathRelative.string());
    assert(false);
    return nullptr;
  }

  auto fragmentShaderName = shaderName + kExtFragment;
  auto fragmentShaderPathRelative = kShaderRoot / fragmentShaderName;
  auto fragmentShaderString = FileManager::instance().readFile(fragmentShaderPathRelative.string());
  fragmentShaderString = resolveSahderIncludes(fragmentShaderString);
  fragmentShaderString = injectDefines(fragmentShaderString, getShaderDefines(key.features));
  if (fragmentShaderString.empty()) {
    Core::Logger::error("ShaderManager", "Failed to load fragment shader: ", fragmentShaderPathRelative.string());
    assert(false);
    return nullptr;
  }

  auto [it, success] =
      _shaderPrograms.emplace(key, std::make_shared<Render::ShaderProgram>(vertexShaderString, fragmentShaderString));

  if (!success) {
    Core::Logger::error("ShaderManager", "Failed to create shader program: ", vertexShaderPathRelative.string(),
                        " and ", fragmentShaderPathRelative.string());
    assert(false);
    return nullptr;
  }

  return it->second;
}
}  // namespace Resources