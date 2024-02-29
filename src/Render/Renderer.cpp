#include "Renderer.h"

namespace Render {


	void Renderer::begin(GLenum mode)
	{
		_currMode = mode;
	}

	void Renderer::end()
	{
		VertexBuffer VBO(_elementsBuff, _vec3Col4, VertexBuffer::_e_Usage::Static);
		_VAO.addBuffer(VBO);
	}

	void Render::Renderer::drawArrays()
	{
		_VAO.bind();
		glDrawArrays(_currMode, 0, _vertexCount);
		_VAO.unbind();
	}
}