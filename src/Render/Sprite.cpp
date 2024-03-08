/*
**  File        :	Sprite.cpp
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#include "Sprite.h"

namespace Render{


	/*
	* �������� �������.
	* �������� - ������ �������� ����������� � �����������
	* ������ - ��������� � ��������� ��� �������� �����������
	*/
	/*============================================================*/
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture2D,
				   const std::string subTextureName,
				   const std::shared_ptr<ShaderProgram> pShaderProgram,
				   const glm::vec2 &spritePosition,
				   const glm::vec2 &spriteSize,
				   const float rotation)
	{
		/* ��������� ������� ����������*/
		_pShaderProgram = std::move(pShaderProgram);
		_pTexture2D = std::move(pTexture2D);
		_position = spritePosition;
		_size = spriteSize;
		_rotation = rotation;

		/*���������� ������� �� 0 �� 1*/
		std::vector<GLfloat> spriteVertexCoords(
													{
														//  2--3    1
														//  | /   / |
														//  1    3--2

														// ������ �����������
														0.f, 0.f,
														0.f, 1.f,
														1.f,1.f,
														// ������ �����������
														1.f,1.f,
														1.f,0.f,
														0.f,0.f
													}
											  );

		/*��������� ��������*/
		auto subTexture = _pTexture2D->getSubTexture2D(subTextureName);

		/*���������� �������� �� 0 �� 1*/
		std::vector<GLfloat> textureVertexCoords(
													{
														// ������ �����������
														subTexture._leftBottomUV.x, subTexture._leftBottomUV.y,
														subTexture._leftBottomUV.x, subTexture._rightTopUV.y,
														subTexture._rightTopUV.x, subTexture._rightTopUV.y,
														// ������ �����������
														subTexture._rightTopUV.x, subTexture._rightTopUV.y,
														subTexture._rightTopUV.x, subTexture._leftBottomUV.y,
														subTexture._leftBottomUV.x, subTexture._leftBottomUV.y
													}
											    );

		std::vector<VertexBuffer::BufferElement> elementsSprite{ Render::VertexBuffer::_e_DataType::Float2 };
		VertexBuffer spriteCoordsVBO(spriteVertexCoords, elementsSprite, VertexBuffer::_e_Usage::Static);
		_VAO.addBuffer(spriteCoordsVBO);

		std::vector<VertexBuffer::BufferElement> elementsTex{ Render::VertexBuffer::_e_DataType::Float2 };
		_pTexVertexVBO = std::make_shared<VertexBuffer>(textureVertexCoords, elementsTex, VertexBuffer::_e_Usage::Static);
		_VAO.addBuffer(*_pTexVertexVBO.get());
	}

	/*============================================================*/
	/*����������*/
	Sprite::~Sprite()
	{
	}

	/*============================================================*/
	/*��������� �������*/
	void Sprite::renderSprite()
	{
		_pShaderProgram->use();

		// ��������� �������
		glm::mat4 modelMatrix(1.f);

		/*
		* ��������� ��������� ������������ ������ 
		* ����������� � �������� ������� (���. ��.)
		* ��������� ����������� ������� ������ ����� �����
		*/

		// ���������� ������ � ������ ��� ����� ������� ��
		modelMatrix = glm::translate(modelMatrix, glm::vec3(_position, 0.f));
		// ���������� ������ � �������������� ���������
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f*_size.x, 0.5*_size.y, 0.f));
		// ������������ ������ �� ������ ����
		modelMatrix = glm::rotate(modelMatrix, glm::radians(_rotation), glm::vec3(0.f, 0.f, 1.f));
		// ��� �������� ���������� ������� �������� ������������ ������� ������ ����, �������
		// ����� ��������� ������ ������ ����� ���, ����������� ����� ����������� ������� � ����� 0 0
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f*_size.x, -0.5*_size.y, 0.f));
		// ������������ ������� �� ���� ����. ������� ������ 1 �� 1 � ����������� �� ������� �������
		modelMatrix = glm::scale(modelMatrix, glm::vec3(_size, 1.f));


		_VAO.bind();
		// ������������� ��������� �������
		_pShaderProgram->setMatrix4Uniform("modelMatrix", modelMatrix);
		// ���������� ������� ���� ��� �������
		glActiveTexture(GL_TEXTURE0);
		// ������ ���������� �������� ��������
		_pTexture2D->bindTexture2D();

		// ��������� ���������� ��� ������������
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// ��������� ����������
		glEnable(GL_BLEND);

		// ������������ ������
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// ���������� ������ ���������� �������
		_VAO.unbind();
		
		// ������������ ����������
		glDisable(GL_BLEND);
	}

	/*============================================================*/
	void Sprite::setSpritePosition(const glm::vec2 &spritePosition)
	{
		_position = spritePosition;
	}

	/*============================================================*/
	void Sprite::setSpriteSize(const glm::vec2 &spriteSize)
	{
		_size = spriteSize;
	}

	/*============================================================*/
	void Sprite::setSpriteRotation(const float rotation)
	{
		_rotation = rotation;
	}

	///*============================================================*/
	//uint64_t Sprite::getFrameDuration(const size_t frameID) const
	//{
	//	if (frameID < _frameParams.size())
	//		return _frameParams[frameID]._duration;

	//	std::cerr << "sprite with this ID does not exist." << std::endl;
	//	return 0;
	//}

	///*============================================================*/
	//size_t Sprite::getFramesCount() const
	//{
	//	return _frameParams.size();
	//}

}