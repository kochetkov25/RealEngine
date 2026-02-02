#include "StaticMeshComponent.h"

#include "Modules/Logger.h"
#include "RendererFactory.h"

namespace Render {
void StaticMeshComponent::upload(std::shared_ptr<Resources::MeshAsset> meshAsset) {
  if (meshAsset == nullptr) {
    Core::Logger::error("StaticMeshComponent", "Mesh asset is nullptr!");
    return;
  }

  if (_renderer != nullptr) {
    Core::Logger::warning("StaticMeshComponent", "Renderer is already set! Will be overwritten!");
  }

  _renderer = RendererFactory::CreateDefault3DModelRenderer();

  for (unsigned int vertexId = 0; vertexId < meshAsset->getVertexCount(); ++vertexId) {
    const auto& vertex = meshAsset->getVertices()[vertexId];
    _renderer->vertex(vertex);

    const auto& texCoord = meshAsset->getTexCoords()[vertexId];
    _renderer->vertexUV(texCoord);

    const auto& normal = meshAsset->getNormals()[vertexId];
    _renderer->normal(normal);
  }

  _renderer->setIndices(meshAsset->getIndices());

  _renderer->upload();
}

void StaticMeshComponent::draw() const {
  if (_renderer == nullptr) {
    Core::Logger::error("StaticMeshComponent", "Renderer is null! Cannot draw!");
    return;
  }
  _renderer->drawElements();
}
}  // namespace Render