#pragma once

#include <glad/glad.h>

#include <vector>
#include <iostream>

namespace Render 
{


	class VertexBuffer
	{
	public:
		friend class VertexArray;

		/*������ ���������*/
		enum class _e_Usage
		{
			Static,
			Dynamic,
			Stream
		};

		/*��� ������������ ������*/
		enum class _e_DataType
		{
			Float,
			Float2,
			Float3,
			Float4,
			Int,
			Int2,
			Int3,
			Int4
		};

		/*��������� ��������� ���� ������� ������� (��������: ������� ����������, ������� ����, � ��)*/
		struct BufferElement
		{
			_e_DataType dataType;      // ���������� ��� ����������� �������� � ���-�� ����������� � ����� ��������
			uint32_t    componentType; // ��� ����������
			size_t      size;          // ������ ��������
			size_t      offset;        // �������� ��������

			BufferElement(_e_DataType type) : dataType(type), componentType(GL_FLOAT), 
				                              size(0), offset(0) {};
		};

		/*����������� � �������������*/
		template <typename _T>
		VertexBuffer(const std::vector<_T>& data, std::vector<BufferElement>& elements, _e_Usage usage = _e_Usage::Static) :
			_elements(std::move(elements)),
			_usage(usage)
		{
			size_t offset = 0;
			/*���������� ���������� ���������*/
			for (auto& element : _elements)
			{
				element.componentType = getComponentType(element.dataType);
				element.size = getElementSize(element.dataType);
				element.offset = offset;
				offset += element.size * sizeof(_T); // �������� �������� � �����
				_stride += element.size * sizeof(_T); // ����� �������� � �����
			}

			glGenBuffers(1, &_id); // ��������� ������
			glBindBuffer(GL_ARRAY_BUFFER, _id); // ����� ������ ��������
			/*������ ������ ���������� � �����*/
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(_T), data.data(), getGLenum(usage)); // ��������� � ���� ������ � ��������
		}

		/*���������� ������ � �������*/
		template <typename _T>
		void updateData(const std::vector<_T>& data)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _id);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(_T), data.data(), getGLenum(_usage));
		}

		/*����������*/
		~VertexBuffer();

		/*����������� �� ���������*/
		VertexBuffer() = delete;

		/*������������ � ���������� ������������*/
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) = delete;

		/*������������ � ���������� ������������*/
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&&) = delete;

		void bind() const;
		static void unbind();
		size_t getStride();
	private:
		/*���������� ������������� �������*/
		unsigned int _id = 0;
		/*������ ��������� ������ ������� ������� (��������: ����������, ����)*/
		std::vector<BufferElement> _elements;
		/*��������: ����� � ����� ���� ��������� ����� ������� (��������: x, y, z, r, g, b, a)*/
		size_t _stride = 0;
		_e_Usage _usage;

		GLenum getGLenum(const _e_Usage usage);

		/*��������� GL ��� ����������*/
		unsigned int getComponentType(_e_DataType type);
		/*��������� ������ ��������*/
		unsigned int getElementSize(_e_DataType type);
	};


}