#include "Renderer.h"

namespace Render 
{


	/*============================================================*/
	/*начало задания координат. Установка mode*/
	void Renderer::begin(GLenum mode)
	{
		_currMode = mode;
	}

	/*============================================================*/
	/*конец задания координат*/
	void Renderer::end()
	{
		std::vector<Render::VertexBuffer::BufferElement> elem;
		/*задаем расположение элементов в буффере в зависимости от State*/
		switch (_renderState)
		{
			case Render::Renderer::State::TEXTURE_2D:
			{
				elem = _vec3UV2;
				break;
			}
			case Render::Renderer::State::PRIMITIVES_GL:
			{
				elem = _vec3Col4;
				break;
			}
			default:
			{
				elem = _vec3Col4;
				break;
			}
		}
		VertexBuffer VBO(_elementsBuff, elem, VertexBuffer::_e_Usage::Static);
		_VAO.addBuffer(VBO);
	}

	/*============================================================*/
	/*отрисовка примитивов по вершинам*/
	void Render::Renderer::drawArrays()
	{
		_VAO.bind();
		glDrawArrays(_currMode, 0, _vertexCount);
		_VAO.unbind();
	}

	/*============================================================*/
	/*установка типа отрисовываемого примитива*/
	void Render::Renderer::Enable(State currState)
	{
		_renderState = currState;
	}


}