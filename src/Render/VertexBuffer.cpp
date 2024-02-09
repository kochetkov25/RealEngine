#include "VertexBuffer.h"

namespace Render
{


	/*============================================================*/
	/*������� �� usage ������ � usage OpenGL*/
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
	/*����������� ������� OpenGL*/
	Render::VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &_id);
	}

	/*============================================================*/
	/*������ ������ ��������*/
	void Render::VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	/*============================================================*/
	/*������������ ������*/
	void Render::VertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*============================================================*/
	/*���������� ��� ���������� �� ����������� ����*/
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
	/*���������� ������ �������� �� ����������� ����*/
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
	/*������ ������ ��������*/
	size_t Render::VertexBuffer::getStride()
	{
		return _stride;
	}
}


