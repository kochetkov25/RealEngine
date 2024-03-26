#include "Renderer.h"

namespace Render 
{


	/*============================================================*/
	/*������ ������� ���������. ��������� mode*/
	void Renderer::begin(GLenum mode)
	{
		_currMode = mode;
	}

	/*============================================================*/
	/*����� ������� ���������*/
	void Renderer::end()
	{
		std::vector<Render::VertexBuffer::BufferElement> elem;
		/*������ ������������ ��������� � ������� � ����������� �� State*/
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
	/*��������� ���������� �� ��������*/
	void Render::Renderer::drawArrays()
	{
		_VAO.bind();
		glDrawArrays(_currMode, 0, _vertexCount);
		_VAO.unbind();
	}

	/*============================================================*/
	/*��������� ���� ��������������� ���������*/
	void Render::Renderer::Enable(State currState)
	{
		_renderState = currState;
	}


}