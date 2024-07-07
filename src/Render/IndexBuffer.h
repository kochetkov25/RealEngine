#include "VertexBuffer.h"


namespace Render
{


	class IndexBuffer
	{
	public:
		/*конструктор*/
		template <typename _T>
		IndexBuffer(const std::vector<_T>& data, VertexBuffer::_e_Usage usage = VertexBuffer::_e_Usage::Static)
		{
			glGenBuffers(1, &_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(_T), data.data(), VertexBuffer::getGLenum(usage));
		}
		/*деструктор*/
		~IndexBuffer();

		/*конструктор по умолчанию*/
		IndexBuffer() = delete;

		/*перемещающий и копирующий конструкторы*/
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&&) = delete;

		/*перемещающее и копирующее присваивание*/
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&&) = delete;

		/*сделать данный буффер активным*/
		void bind() const;
		/*отвязать буффер*/
		static void unbind();

	private:
		unsigned int _id = 0;
	};


}