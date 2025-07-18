#include "RendererFactory.h"

namespace Render {
	std::shared_ptr<Renderer> RendererFactory::CreateDefault3DModelRenderer() {
		auto renderer = std::shared_ptr<Renderer>(new Renderer());

		renderer->setDrawMode(GL_TRIANGLES);
		renderer->setLayout({
			Renderer::Float3, // vertex
			Renderer::Float2, // tex coords (UV)
			Renderer::Float3, // normals
			});

		return renderer;
	}

	std::shared_ptr<Renderer> RendererFactory::CreateTextureRenderer() {
		auto renderer = std::shared_ptr<Renderer>(new Renderer());

		renderer->setDrawMode(GL_TRIANGLES);
		renderer->setLayout({
			Renderer::Float3, // vertex
			Renderer::Float2, // tex coords (UV)
			});

		return renderer;
	}

	std::shared_ptr<Renderer> RendererFactory::CreatePrimitivesRenderer() {
		auto renderer = std::shared_ptr<Renderer>(new Renderer());

		renderer->setDrawMode(GL_TRIANGLES);
		renderer->setLayout({
			Renderer::Float3, // vertex
			Renderer::Float4, // color
			});

		return renderer;
	}

	std::shared_ptr<Renderer> RendererFactory::CreateDebugRenderer() {
		auto renderer = std::shared_ptr<Renderer>(new Renderer());

		renderer->setDrawMode(GL_TRIANGLES);
		renderer->setLayout({
			Renderer::Float3, // vertex
			});

		return renderer;
	}
}