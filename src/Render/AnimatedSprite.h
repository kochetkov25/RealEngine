#pragma once

#include "Sprite.h"
#include <map>
#include <vector>

namespace Render{

	class AnimatedSprite : public Sprite
	{
	public:
		// �����������. � ��� ��������� ��� ������������� ���� �������� ������ ����� ������ ������������ �������� ������
		AnimatedSprite(const std::shared_ptr<Texture2D> pTexture2D,
			const std::string subTextureName,
			const std::shared_ptr<ShaderProgram> pShaderProgram,
			const glm::vec2 &spritePosition,
			const glm::vec2 &spriteSize,
			const float rotation);
		// ���������� ����� ��������� (��� ��������� � ������ ������� ������ ���� ����������� - ������������ �����������)
		void insertState(std::string state, std::vector<std::pair<std::string, size_t>> subTextureDuration);
		// ��������� �������������� �������
		void renderSprite() override;
		// �������������� ������ ��������
		void update(uint64_t deltaTime);
		// ��������� ���������, ������� ����� ������������
		void setState(std::string newState);
	private:
		// ���, ������� ������ ������ �������� ���������, � ��������� ������ ���� ������ ������
		// (� ������� ������ ���� �������� ��� ��������, � ������������ ����� (� ���� ��������))
		std::map<std::string, std::vector<std::pair<std::string, size_t>>> _statesMap;

		size_t _currentFrame = 0;
		uint64_t  _currentAnimationTime = 0;

		std::map<std::string, std::vector<std::pair<std::string, size_t>>>::const_iterator _pCurrentAnimationDurations;
		// ����, ������� "���������" ����������� �������� ( �������� � true ��� ��������� ������������ �����
		bool _dirty = false;
	};

}