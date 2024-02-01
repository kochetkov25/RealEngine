#include "AnimatedSprite.h"
#include <iostream>
#include "Texture2D.h"

namespace Render{
	// �����������. � ��� ��������� ��� ������������� ���� �������� ������ ����� ������ ������������ �������� ������
	AnimatedSprite::AnimatedSprite(const std::shared_ptr<Texture2D> pTexture2D,
		const std::string subTextureName,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2 &spritePosition,
		const glm::vec2 &spriteSize,
		const float rotation) : Sprite(std::move(pTexture2D), std::move(subTextureName),
									   std::move(pShaderProgram), std::move(spritePosition), 
									   std::move(spriteSize), std::move(rotation))
	{ 
		// ������������� ������� ��������� � "������"  (����� �������� ��� �� ������)
		_pCurrentAnimationDurations = _statesMap.end();
	}

	// ���������� ����� ��������� (��� ��������� � ������ ������� ������ ���� ����������� - ������������ �����������)
	void AnimatedSprite::insertState(std::string state, std::vector<std::pair<std::string, size_t>> subTextureDuration)
	{
		_statesMap.emplace(std::move(state), std::move(subTextureDuration));
	}
	// ��������� �������������� �������
	void AnimatedSprite::renderSprite()
	{
		// ������������ ������, ������ ���� ��������� ����
		if (_dirty)
		{
			auto subTexture = _pTexture2D->getSubTexture2D(_pCurrentAnimationDurations->second[_currentFrame].first);

			// ���� ������������ ������, ������ ����� �������� ������ ���������� �� ����� � �������� �� � ������
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

			glBindBuffer(GL_ARRAY_BUFFER, _textureVertexCoordsVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureVertexCoords), &textureVertexCoords);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			_dirty = false; 
		}
		// �������� ����� �������� ������ (��������� �������)
		Sprite::renderSprite();


	}
	// �������������� ������ ��������
	void AnimatedSprite::update(uint64_t deltaTime)
	{
		// ���� ������� ��������� ����������
		if (_pCurrentAnimationDurations != _statesMap.end())
		{
			// ����������� �����, ������� ������ �������� �� ������� ������
			_currentAnimationTime += deltaTime;

			// ���� ������� ����� �������� ������ ���, �����, ������� ������ ������ ������� ���� ��������
			while (_currentAnimationTime >= _pCurrentAnimationDurations->second[_currentFrame].second)
			{
				// ��������� ������� ����� �� ����� ������������ �������� �����
				_currentAnimationTime -= _pCurrentAnimationDurations->second[_currentFrame].second;
				// ������������� ����
				++_currentFrame;
				// ������������� ����, ������� ��������� �����������
				_dirty = true;
				// ���� ����� �� ���������� �����, �������� �������� � ������
				if (_currentFrame == _pCurrentAnimationDurations->second.size())
				{
					_currentFrame = 0;
				}
			}
		}
	}
	// ��������� ���������, ������� ����� ������������
	void AnimatedSprite::setState(std::string newState)
	{
		auto it = _statesMap.find(newState);
		if (it == _statesMap.end())
		{
			std::cerr << "can not find animation state: " << newState << std::endl;
			return;
		}
		// ���� ����� ���������, �� �� �� �����, ��� ����, ������������� ��� � ����
		if (it != _pCurrentAnimationDurations)
		{
			_currentAnimationTime = 0;
			_currentFrame = 0;
			_pCurrentAnimationDurations = it;
			_dirty = true;
		}
	}
}