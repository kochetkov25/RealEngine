
#include "VertexArray.h"

#include "glad/glad.h"

namespace Render
{


	/*============================================================*/
	/*в конструкторе генерируем буффер*/
	Render::VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &_id);
	}

	/*============================================================*/
	/*в деструкторе освобождаем ресурсы*/
	Render::VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &_id);
	}

	/*============================================================*/
	/*делаем буффер активным*/
	void Render::VertexArray::bind() const
	{
		glBindVertexArray(_id);
	}

	/*============================================================*/
	/*отвязываем буффер*/
	void Render::VertexArray::unbind()
	{
		glBindVertexArray(0); 
	}

	/*============================================================*/
	/*ассоциируем ВАО с ВБО*/
	void Render::VertexArray::addBuffer(VertexBuffer& vertexBuffer)
	{

		bind();
		vertexBuffer.bind();

		/*все необходимые параметры уже определены в ВБО*/
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

}