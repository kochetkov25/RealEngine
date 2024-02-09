#pragma once

#include "VertexBuffer.h"

namespace Render
{



	class VertexArray
	{
	public:
		/*конструктор по умолчанию*/
		VertexArray();
		/*деструктор*/
		~VertexArray();

		/*перемещающий и копирующий конструкторы*/
		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&&) = delete;

		/*перемещающее и копирующее присваивание*/
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&&) = delete;

		/*ассоциируем ВАО с ВБО*/
		void addBuffer(VertexBuffer& vertexBuffer);

		/*делаем ВАО активным*/
		void bind() const;
		/*отвязываем ВАО*/
		static void unbind();

	private:
		/*уникальный идентификатор для ВАО*/
		unsigned int _id = 0;
		/*текущий лейаут (для шейдерной программы)*/
		unsigned int _layoutLocation = 0;
	};



}