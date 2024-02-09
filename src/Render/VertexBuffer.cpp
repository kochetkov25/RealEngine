#include "VertexBuffer.h"

namespace Render
{


	/*============================================================*/
	/*перевод из usage класса в usage OpenGL*/
	GLenum Render::VertexBuffer::getGLenum(const _e_Usage usage)
	{
		switch (usage)
		{
		case _e_Usage::Static: return GL_STATIC_DRAW;
		case _e_Usage::Dynamic: return GL_DYNAMIC_DRAW;
		case _e_Usage::Stream: return GL_STREAM_DRAW;
		default:
			std::cerr << "Unknow usage" << std::endl;
			return GL_STREAM_DRAW;
		}
	}

	/*============================================================*/
	/*осаобождаем ресурсы OpenGL*/
	Render::VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	/*============================================================*/
	/*делаем буффер активным*/
	void Render::VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	/*============================================================*/
	/*деактивируем буффер*/
	void Render::VertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*============================================================*/
	/*выччисляем тип компоненат по встроенному типу*/
	unsigned int Render::VertexBuffer::getComponentType(_e_DataType type)
	{
		switch (type)
		{
		case Render::VertexBuffer::_e_DataType::Float:
		case Render::VertexBuffer::_e_DataType::Float2:
		case Render::VertexBuffer::_e_DataType::Float3:
		case Render::VertexBuffer::_e_DataType::Float4:
			return GL_FLOAT;
		case Render::VertexBuffer::_e_DataType::Int:
		case Render::VertexBuffer::_e_DataType::Int2:
		case Render::VertexBuffer::_e_DataType::Int3:
		case Render::VertexBuffer::_e_DataType::Int4:
			return GL_INT;
		default:
			std::cerr << "Unknown data type!" << std::endl;
			return GL_FLOAT;
		}
	}

	/*============================================================*/
	/*выччисляем размер элемента по встроенному типу*/
	unsigned int Render::VertexBuffer::getElementSize(_e_DataType type)
	{
		switch (type)
		{
		case Render::VertexBuffer::_e_DataType::Float:
		case Render::VertexBuffer::_e_DataType::Int:
			return 1;
		case Render::VertexBuffer::_e_DataType::Float2:
		case Render::VertexBuffer::_e_DataType::Int2:
			return 2;
		case Render::VertexBuffer::_e_DataType::Float3:
		case Render::VertexBuffer::_e_DataType::Int3:
			return 3;
		case Render::VertexBuffer::_e_DataType::Float4:
		case Render::VertexBuffer::_e_DataType::Int4:
			return 4;
		default:
			std::cerr << "Unknown data type!" << std::endl;
			return 0;
		}
	}

	/*============================================================*/
	/*отдаем наружу смещение*/
	size_t Render::VertexBuffer::getStride()
	{
		return _stride;
	}
}


