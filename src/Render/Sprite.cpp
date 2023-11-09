#include "Sprite.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "glm\mat4x4.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <string>

namespace Render{

	// �������� �������.
	// �������� - ������ �������� ����������� � �����������
	// ������ - ��������� � ��������� ��� �������� �����������
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture2D,
		const std::string subTextureName,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2 &spritePosition,
		const glm::vec2 &spriteSize,
		const float rotation)
	{
		// ��������� ������� ����������
		_pShaderProgram = std::move(pShaderProgram);
		_pTexture2D = std::move(pTexture2D);
		_position = spritePosition;
		_size = spriteSize;
		_rotation = rotation;

		// ���������� ������� �� 0 �� 1
		const GLfloat spriteVertexCoords[] = {
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
		};

		// !!! ������ ������ !!! ������� ����, ������ ����������� �� �����, ���� ������� ����.
		// ���� ����� ����������� ���, ����� ���������� ��� ��������, ��� ��������� ��������� ��� ������� ����� �������, ��� � ������� ����������
		auto subTexture = _pTexture2D->getSubTexture2D(subTextureName);

		// ���������� �������� �� 0 �� 1
		const GLfloat textureVertexCoords[] = {
			// ������ �����������
			subTexture._leftBottomUV.x, subTexture._leftBottomUV.y,
			subTexture._leftBottomUV.x, subTexture._rightTopUV.y,
			subTexture._rightTopUV.x, subTexture._rightTopUV.y,
			// ������ �����������
			subTexture._rightTopUV.x, subTexture._rightTopUV.y,
			subTexture._rightTopUV.x, subTexture._leftBottomUV.y,
			subTexture._leftBottomUV.x, subTexture._leftBottomUV.y
		};
		// ������� ������ ���������� �������
		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		// ������� �������� ������� ��� ��������� ������ �������
		glGenBuffers(1, &_spriteVertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _spriteVertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertexCoords), &spriteVertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// ������� �������� ������� ��� ��������� ������ ��������
		glGenBuffers(1, &_textureVertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _textureVertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertexCoords), &textureVertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// ���������� ������� � ��������� �������� OpenGl � ������� ���������
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Sprite::~Sprite()
	{
		// ������������ ������ ����������
		glDeleteBuffers(1, &_spriteVertexCoordsVBO);
		glDeleteBuffers(1, &_textureVertexCoordsVBO);
		glDeleteVertexArrays(1, &_VAO);
	}

	void Sprite::renderSprite()
	{
		_pShaderProgram->use();

		// ��������� �������
		glm::mat4 modelMatrix(1.f);

		// ��������� ��������� ������������ ������ ����������� � �������� �������
		// ��������� ������������ ������� ������ ����� �����

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


		glBindVertexArray(_VAO);
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
		glBindVertexArray(0);
		
		// ������������ ����������
		glDisable(GL_BLEND);
	}
	void Sprite::setSpritePosition(const glm::vec2 &spritePosition)
	{
		_position = spritePosition;
	}
	void Sprite::setSpriteSize(const glm::vec2 &spriteSize)
	{
		_size = spriteSize;
	}
	void Sprite::setSpriteRotation(const float rotation)
	{
		_rotation = rotation;
	}
}