#include "RendererFactory.h"

#include "Renderer.h"

namespace Render {
std::shared_ptr<Renderer> RendererFactory::CreateDefault3DModelRenderer() {
  auto renderer = std::shared_ptr<Renderer>(new Renderer());

  renderer->setDrawMode(GL_TRIANGLES);
  renderer->setLayout({
      Renderer::Float3,  // vertex
      Renderer::Float2,  // tex coords (UV)
      Renderer::Float3,  // normals
  });

  return renderer;
}

std::shared_ptr<Renderer> RendererFactory::CreateSkinned3DModelRenderer() {
  auto renderer = std::shared_ptr<Renderer>(new Renderer());

  renderer->setDrawMode(GL_TRIANGLES);
  renderer->setLayout({
      Renderer::Float3,  // vertex
      Renderer::Float2,  // tex coords (UV)
      Renderer::Float3,  // normals
      Renderer::UInt4,   // bone ids
      Renderer::Float4,  // bone weights
  });

  return renderer;
}

std::shared_ptr<Renderer> RendererFactory::CreateTextureRenderer() {
  auto renderer = std::shared_ptr<Renderer>(new Renderer());

  renderer->setDrawMode(GL_TRIANGLES);
  renderer->setLayout({
      Renderer::Float3,  // vertex
      Renderer::Float2,  // tex coords (UV)
  });

  return renderer;
}

std::shared_ptr<Renderer> RendererFactory::CreatePrimitivesRenderer() {
  auto renderer = std::shared_ptr<Renderer>(new Renderer());

  renderer->setDrawMode(GL_TRIANGLES);
  renderer->setLayout({
      Renderer::Float3,  // vertex
      Renderer::Float4,  // color
  });

  return renderer;
}

std::shared_ptr<Renderer> RendererFactory::CreateDebugRenderer() {
  auto renderer = std::shared_ptr<Renderer>(new Renderer());

  renderer->setDrawMode(GL_TRIANGLES);
  renderer->setLayout({
      Renderer::Float3,  // vertex
  });

  return renderer;
}

std::shared_ptr<Renderer> RendererFactory::CreateDebugGridRenderer() {
  auto renderer = std::shared_ptr<Renderer>(new Renderer());
  renderer->setDrawMode(GL_TRIANGLES);
  renderer->setLayout({
      Renderer::Float3,  // vertex
  });

  const size_t kNumVerts4Triangle = 3;
  for (auto numVert = 0; numVert < kNumVerts4Triangle; numVert++) {
    renderer->vertex3(0.f, 0.f, 0.f);
  }

  return renderer;
}
}  // namespace Render