#pragma once

#include "VertexBuffer.h"

namespace Render
{



	class VertexArray
	{
	public:
		/*����������� �� ���������*/
		VertexArray();
		/*����������*/
		~VertexArray();

		/*������������ � ���������� ������������*/
		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&&) = delete;

		/*������������ � ���������� ������������*/
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&&) = delete;

		/*����������� ��� � ���*/
		void addBuffer(VertexBuffer& vertexBuffer);

		/*������ ��� ��������*/
		void bind() const;
		/*���������� ���*/
		static void unbind();

	private:
		/*���������� ������������� ��� ���*/
		unsigned int _id = 0;
		/*������� ������ (��� ��������� ���������)*/
		unsigned int _layoutLocation = 0;
	};



}