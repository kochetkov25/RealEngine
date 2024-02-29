#pragma once

#include <glad/glad.h>

#include "VertexArray.h"

#include <vector>

static const size_t MAX_ELEMENTS = 1024 * 2;

namespace Render {

	class Renderer
	{
	public:
		Renderer()
		{
			_elementsBuff.reserve(MAX_ELEMENTS);
			_currMode = GL_TRIANGLES;
			_vertexCount = 0;
		}
		/*добавление в буффер вершины из трех координат*/
		template<typename _T>
		void verex3(_T x, _T y, _T z)
		{
			_elementsBuff.emplace_back(static_cast<float>(x));
			_elementsBuff.emplace_back(static_cast<float>(y));
			_elementsBuff.emplace_back(static_cast<float>(z));

			_vertexCount++;
		}

		/*добавление в буффер цвета для вершины в формате RGBA*/
		template<typename _T>
		void color4(_T r, _T g, _T b, _T a = 1.f)
		{
			_elementsBuff.emplace_back(static_cast<float>(r) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(g) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(b) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(a));
		}

		void begin(GLenum mode);

		void end();

		void drawArrays();
	private:
		/*буффер под вершины и цвета*/
		std::vector<float> _elementsBuff;
		size_t _vertexCount;
		/*тип элементов в буффере*/
		std::vector<VertexBuffer::BufferElement> _vec3Col4 = {
															   VertexBuffer::_e_DataType::Float3,
															   VertexBuffer::_e_DataType::Float4
		                                                     };

		GLenum _currMode;

		VertexArray _VAO;
	};
}