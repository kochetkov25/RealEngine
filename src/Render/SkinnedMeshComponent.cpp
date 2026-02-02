#include "SkinnedMeshComponent.h"

#include "Modules/Logger.h"
#include "RendererFactory.h"

namespace Render {
void SkinnedMeshComponent::upload(std::shared_ptr<Resources::MeshAsset> meshAsset) {
  if (meshAsset == nullptr) {
    Core::Logger::error("SkinnedMeshComponent", "Mesh asset is nullptr!");
    return;
  }

  if (_renderer != nullptr) {
    Core::Logger::warning("SkinnedMeshComponent", "Renderer is already set! Will be overwritten!");
  }

  _renderer = RendererFactory::CreateSkinned3DModelRenderer();

  for (unsigned int vertexId = 0; vertexId < meshAsset->getVertexCount(); ++vertexId) {
    const auto& vertex = meshAsset->getVertices()[vertexId];
    _renderer->vertex(vertex);

    const auto& texCoord = meshAsset->getTexCoords()[vertexId];
    _renderer->vertexUV(texCoord);

    const auto& normal = meshAsset->getNormals()[vertexId];
    _renderer->normal(normal);

    const auto& boneData = meshAsset->getBoneData().value()[vertexId];
    _renderer->boneIDs(boneData.boneIds[0], boneData.boneIds[1], boneData.boneIds[2], boneData.boneIds[3]);
    _renderer->boneWeights(boneData.boneWeights[0], boneData.boneWeights[1], boneData.boneWeights[2],
                           boneData.boneWeights[3]);
  }

  _renderer->setIndices(meshAsset->getIndices());

  _renderer->upload();
}

void SkinnedMeshComponent::draw() const {
  if (_renderer == nullptr) {
    Core::Logger::error("SkinnedMeshComponent", "Renderer is null! Cannot draw!");
    return;
  }
  _renderer->drawElements();
}
}  // namespace Render