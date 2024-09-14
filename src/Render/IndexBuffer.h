#pragma once

#include "VertexBuffer.h"

namespace Render
{


	class IndexBuffer
	{
	public:
		/*�����������*/
		template <typename _T>
		IndexBuffer(const std::vector<_T>& data, VertexBuffer::_e_Usage usage = VertexBuffer::_e_Usage::Static)
		{
			glGenBuffers(1, &_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(_T), data.data(), VertexBuffer::getGLenum(usage));
		}
		/*����������*/
		~IndexBuffer();

		/*����������� �� ���������*/
		IndexBuffer() = delete;

		/*������������ � ���������� ������������*/
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&&) = delete;

		/*������������ � ���������� ������������*/
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&&) = delete;

		/*������� ������ ������ ��������*/
		void bind() const;
		/*�������� ������*/
		static void unbind();

	private:
		unsigned int _id = 0;
	};


}