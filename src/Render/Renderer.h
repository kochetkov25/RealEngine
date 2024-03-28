#pragma once

#include <glad/glad.h>

#include "VertexArray.h"

#include <vector>
#include <string>

static const size_t MAX_ELEMENTS = 1024 * 2;

namespace Render {

	class Renderer
	{
	public:
		/*��� �������������� ����������*/
		enum class State
		{
			TEXTURE_2D,
			PRIMITIVES_GL,
			TEXTURE_2D_LIGHT
		};

		/*�����������*/
		Renderer()
		{
			_elementsBuff.reserve(MAX_ELEMENTS);
			_currMode = GL_TRIANGLES;
			_vertexCount = 0;
			_renderState = State::PRIMITIVES_GL;
		}
		/*���������� � ������ ������� �� ���� ���������*/
		template<typename _T>
		void verex3(_T x, _T y, _T z)
		{
			_elementsBuff.emplace_back(static_cast<float>(x));
			_elementsBuff.emplace_back(static_cast<float>(y));
			_elementsBuff.emplace_back(static_cast<float>(z));

			_vertexCount++;
		}

		/*���������� � ������ ����� ��� ������� � ������� RGBA (�� 0 �� 255)*/
		template<typename _T>
		void color4(_T r, _T g, _T b, _T a = 1.f)
		{
			_elementsBuff.emplace_back(static_cast<float>(r) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(g) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(b) / 255.f);
			_elementsBuff.emplace_back(static_cast<float>(a));
		}

		/*��������� � ������ ������� ������� (�� 0 �� 1)*/
		template<typename _T>
		void vertexUV(_T U, _T V)
		{
			_elementsBuff.emplace_back(static_cast<float>(U));
			_elementsBuff.emplace_back(static_cast<float>(V));
		}

		/*������ ������� ����������*/
		void begin(GLenum mode);

		/*��������� ������� ����������*/
		void end();

		/*���������� ��� �������*/
		void drawArrays();

		void Enable(State currState);
	private:
		/*������ ��� ������� � �����*/
		std::vector<float> _elementsBuff;
		size_t _vertexCount;
		/*��� ��������� � ������� ��� ����������*/
		std::vector<VertexBuffer::BufferElement> _vec3Col4 = {
															   VertexBuffer::_e_DataType::Float3,
															   VertexBuffer::_e_DataType::Float4
		                                                     };
		/*��� ��������� � ������� ��� ��������*/
		std::vector<VertexBuffer::BufferElement> _vec3UV2 = {
																VertexBuffer::_e_DataType::Float3,
																VertexBuffer::_e_DataType::Float2
															};
		/*��� ��������� � ������� ��� �������� � ����������*/
		std::vector<VertexBuffer::BufferElement> _vec3UV2vec3 = {
																VertexBuffer::_e_DataType::Float3,
																VertexBuffer::_e_DataType::Float2,
																VertexBuffer::_e_DataType::Float3,
		                                                    };
		/*������� ����� ���������*/
		GLenum _currMode;

		/*���*/
		VertexArray _VAO;

		/*��������� ������ ��� ��������������� ���������*/
		State _renderState;
	};
}