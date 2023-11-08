#pragma once

#include "Sprite.h"
#include <map>
#include <vector>

namespace Render{

	class AnimatedSprite : public Sprite
	{
	public:
		// конструктор. В нем заполняем все наследованные поля базового класса путем вызова конструктора базового класса
		AnimatedSprite(const std::shared_ptr<Texture2D> pTexture2D,
			const std::string subTextureName,
			const std::shared_ptr<ShaderProgram> pShaderProgram,
			const glm::vec2 &spritePosition,
			const glm::vec2 &spriteSize,
			const float rotation);
		// определяем новое состояние (имя состояния и вектор который хранит пары сабтекстура - длительность сабтекстуры)
		void insertState(std::string state, std::vector<std::pair<std::string, size_t>> subTextureDuration);
		// отрисовка анимированного спрайта
		void renderSprite() override;
		// перещелкивание кадров анимации
		void update(uint64_t deltaTime);
		// установка состояния, которое хотим отрисовывать
		void setState(std::string newState);
	private:
		// мап, который хранит ключом название состояния, а значением хранит пару вектор кадров
		// (в векторе кадров есть название саб текстуры, и длительность кадра (в нано секундах))
		std::map<std::string, std::vector<std::pair<std::string, size_t>>> _statesMap;

		size_t _currentFrame = 0;
		uint64_t  _currentAnimationTime = 0;

		std::map<std::string, std::vector<std::pair<std::string, size_t>>>::const_iterator _pCurrentAnimationDurations;
		// флаг, который "разрешает" перерисовку текстуры ( ставится в true при истечении длительности кадра
		bool _dirty = false;
	};

}