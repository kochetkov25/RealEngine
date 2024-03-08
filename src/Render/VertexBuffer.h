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

		/*способ отрисовки*/
		enum class _e_Usage
		{
			Static,
			Dynamic,
			Stream
		};

		/*тип передаваемых данных*/
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

		/*структура описывает один элемент вершины (Например: элемент координаты, элемент цвет, и тд)*/
		struct BufferElement
		{
			_e_DataType dataType;      // определяет тип компонентов элементы и кол-во компонентов в одном элементе
			uint32_t    componentType; // тип компонента
			size_t      size;          // размер элемента
			size_t      offset;        // смещение элемента

			BufferElement(_e_DataType type) : dataType(type), componentType(GL_FLOAT), 
				                              size(0), offset(0) {};
		};

		/*конструктор и инициализация*/
		template <typename _T>
		VertexBuffer(const std::vector<_T>& data, std::vector<BufferElement>& elements, _e_Usage usage = _e_Usage::Static) :
			_elements(std::move(elements)),
			_usage(usage)
		{
			size_t offset = 0;
			/*вычисление параметров элементов*/
			for (auto& element : _elements)
			{
				element.componentType = getComponentType(element.dataType);
				element.size = getElementSize(element.dataType);
				element.offset = offset;
				offset += element.size * sizeof(_T); // смещение задается в битах
				_stride += element.size * sizeof(_T); // сдвиг задается в битах
			}

			glGenBuffers(1, &_id); // генерирую буффер
			glBindBuffer(GL_ARRAY_BUFFER, _id); // делаю буффер активным
			/*размер данных передается в битах*/
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(_T), data.data(), getGLenum(usage)); // перемещаю в него данные о вершинах
		}

		/*обновление данных в буффере*/
		template <typename _T>
		void updateData(const std::vector<_T>& data)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _id);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(_T), data.data(), getGLenum(_usage));
		}

		/*деструктор*/
		~VertexBuffer();

		/*конструктор по умолчанию*/
		VertexBuffer() = delete;

		/*перемещающий и копирующий конструкторы*/
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) = delete;

		/*перемещающее и копирующее присваивание*/
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&&) = delete;

		void bind() const;
		static void unbind();
		size_t getStride();
	private:
		/*уникальный идентификатор буффера*/
		unsigned int _id = 0;
		/*вектор описывает каждые элемент вершины (Например: координаты, цвет)*/
		std::vector<BufferElement> _elements;
		/*смещение: длина в битах всех атрибутов ОДНОЙ вершины (Например: x, y, z, r, g, b, a)*/
		size_t _stride = 0;
		_e_Usage _usage;

		GLenum getGLenum(const _e_Usage usage);

		/*вычисляет GL тип компонента*/
		unsigned int getComponentType(_e_DataType type);
		/*вычисляет размер элемента*/
		unsigned int getElementSize(_e_DataType type);
	};


}