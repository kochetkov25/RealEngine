
#include "VertexArray.h"

#include "IndexBuffer.h"

#include "glad/glad.h"

namespace Render
{

	/*============================================================*/
	/*� ������������ ���������� ������*/
	Render::VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &_id);
	}

	/*============================================================*/
	/*� ����������� ����������� �������*/
	Render::VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &_id);
	}

	/*============================================================*/
	/*������ ������ ��������*/
	void Render::VertexArray::bind() const
	{
		glBindVertexArray(_id);
	}

	/*============================================================*/
	/*���������� ������*/
	void Render::VertexArray::unbind()
	{
		glBindVertexArray(0); 
	}

	/*============================================================*/
	/*����������� ��� � ���*/
	void Render::VertexArray::addBuffer(VertexBuffer& vertexBuffer)
	{

		bind();
		vertexBuffer.bind();

		/*��� ����������� ��������� ��� ���������� � ���*/
		for (const VertexBuffer::BufferElement& element: vertexBuffer._elements)
		{
			glEnableVertexAttribArray(_layoutLocation);
			glVertexAttribPointer(_layoutLocation,
				                  element.size,
				                  static_cast<GLenum>(element.componentType),
				                  GL_FALSE,
				                  vertexBuffer.getStride(),
				                  reinterpret_cast<const void*>(element.offset));
			_layoutLocation++;
		}

		unbind();
	}

	void VertexArray::setIndexBuffer(IndexBuffer& indexBuffer)
	{
		bind();
		indexBuffer.bind();
		unbind();
	}

}