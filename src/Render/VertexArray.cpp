
#include "VertexArray.h"

#include "IndexBuffer.h"

#include "glad/glad.h"

namespace Render
{

	// Constructor: generates a new vertex array object
	Render::VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &_id);
	}

	// Destructor: deletes the vertex array object
	Render::VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &_id);
	}

	// Bind the vertex array object
	void Render::VertexArray::bind() const
	{
		glBindVertexArray(_id);
	}

	// Unbind the vertex array object
	void Render::VertexArray::unbind()
	{
		glBindVertexArray(0); 
	}

	// Add a vertex buffer to this vertex array and configure vertex attributes
	void Render::VertexArray::addBuffer(VertexBuffer& vertexBuffer)
	{

		bind();
		vertexBuffer.bind();

		// Configure vertex attributes for each element in the buffer layout
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
