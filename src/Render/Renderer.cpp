#include "Renderer.h"

#include "IndexBuffer.h"

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
			case Render::Renderer::State::TEXTURE_2D_LIGHT:
			{
				elem = _vec3UV2vec3;
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
	/*установить порядок индексов для отрисовки*/
	void Renderer::setIndices(const std::vector<GLuint>& indices)
	{
		IndexBuffer IBO(indices);
		_indicesCount = indices.size();

		_VAO.setIndexBuffer(IBO);
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
	/*отрисовать все вершины с использованием индексов вершин*/
	void Render::Renderer::drawElements()
	{
		_VAO.bind();
		glDrawElements(_currMode, _indicesCount, GL_UNSIGNED_INT, 0);
		_VAO.unbind();
	}

	/*============================================================*/
	/*установка типа отрисовываемого примитива*/
	void Render::Renderer::Enable(State currState)
	{
		_renderState = currState;
	}


}