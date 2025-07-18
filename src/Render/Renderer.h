#pragma once

#include <glad/glad.h>

#include "VertexArray.h"

#include <vector>
#include <string>

namespace Render {
	static const size_t MAX_ELEMENTS = 1024 * 2;

	static const size_t MAX_LAYOUT = 10;

	class Renderer {
		friend class RendererFactory;
	public:
		enum DataType {
			Float,
			Float2,
			Float3,
			Float4,
			Int,
			Int2,
			Int3,
			Int4
		};

		// Appends a vertex (x, y, z) to the buffer
		template<typename _T>
		void verex3(_T x, _T y, _T z) {
			_elementsBuff.emplace_back(static_cast<float>(x));
			_elementsBuff.emplace_back(static_cast<float>(y));
			_elementsBuff.emplace_back(static_cast<float>(z));

			_vertexCount++;
		}

		// Adds RGBA vertex color (0–255 range) to the buffer
		template<typename _T>
		void color4(_T r, _T g, _T b, _T a = 1.f) {
			_elementsBuff.emplace_back(static_cast<float>(r) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(g) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(b) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(a));
		}

		// Appends sprite UV coordinates (normalized 0–1) to the buffer
		template<typename _T>
		void vertexUV(_T U, _T V) {
			_elementsBuff.emplace_back(static_cast<float>(U));
			_elementsBuff.emplace_back(static_cast<float>(V));
		}

		// Sets the vertex layout describing the structure of vertex data.
		void setLayout(const std::vector<DataType>& layout);

		// Sets the OpenGL primitive draw mode (e.g., GL_TRIANGLES, GL_LINES).
		void setDrawMode(GLenum mode);

		// Uploads the vertex data and layout to the GPU.
		void upload();

		// Sets the index buffer for indexed rendering.
		void setIndices(const std::vector<GLuint>& indices);

		// Draws the uploaded vertex buffer using the configured draw mode.
		void drawArrays();

		// Draws geometry using the uploaded vertex and index buffers.
		void drawElements();

	private:
		// ctor
		explicit Renderer() : _drawMode(GL_TRIANGLES), _vertexCount(0), _indicesCount(0) {
			_elementsBuff.reserve(MAX_ELEMENTS);
			_layout.reserve(MAX_LAYOUT);
		}

		std::vector<float> _elementsBuff;
		size_t _vertexCount;
		size_t _indicesCount;

		GLenum _drawMode;

		VertexArray _VAO;

		std::vector<VertexBuffer::BufferElement> _layout;
	};
}