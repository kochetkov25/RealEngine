#include "AnimatedSprite.h"
#include <iostream>
#include "Texture2D.h"

namespace Render{
	// конструктор. В нем заполняем все наследованные поля базового класса путем вызова конструктора базового класса
	AnimatedSprite::AnimatedSprite(const std::shared_ptr<Texture2D> pTexture2D,
		const std::string subTextureName,
		const std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2 &spritePosition,
		const glm::vec2 &spriteSize,
		const float rotation) : Sprite(std::move(pTexture2D), std::move(subTextureName),
									   std::move(pShaderProgram), std::move(spritePosition), 
									   std::move(spriteSize), std::move(rotation))
	{ 
		// устанавливаем текущее состояние в "ничего"  (чтобы итератор был не пустой)
		_pCurrentAnimationDurations = _statesMap.end();
	}

	// определяем новое состояние (имя состояния и вектор который хранит пары сабтекстура - длительность сабтекстуры)
	void AnimatedSprite::insertState(std::string state, std::vector<std::pair<std::string, size_t>> subTextureDuration)
	{
		_statesMap.emplace(std::move(state), std::move(subTextureDuration));
	}
	// отрисовка анимированного спрайта
	void AnimatedSprite::renderSprite()
	{
		// отрисовываем спрайт, только если позволяет флаг
		if (_dirty)
		{
			auto subTexture = _pTexture2D->getSubTexture2D(_pCurrentAnimationDurations->second[_currentFrame].first);

			// если отрисовываем спрайт, значит нужно изменить старые координаты на новые и передать их в буффер
			// координаты текстуры от 0 до 1
			const GLfloat textureVertexCoords[] = {
				// первый треугольник
				subTexture._leftBottomUV.x, subTexture._leftBottomUV.y,
				subTexture._leftBottomUV.x, subTexture._rightTopUV.y,
				subTexture._rightTopUV.x, subTexture._rightTopUV.y,
				// второй треугольник
				subTexture._rightTopUV.x, subTexture._rightTopUV.y,
				subTexture._rightTopUV.x, subTexture._leftBottomUV.y,
				subTexture._leftBottomUV.x, subTexture._leftBottomUV.y
			};

			glBindBuffer(GL_ARRAY_BUFFER, _textureVertexCoordsVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureVertexCoords), &textureVertexCoords);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			_dirty = false; 
		}
		// вызываем метод базового класса (отрисовка спрайта)
		Sprite::renderSprite();


	}
	// перещелкивание кадров анимации
	void AnimatedSprite::update(uint64_t deltaTime)
	{
		// если текущее состояние существует
		if (_pCurrentAnimationDurations != _statesMap.end())
		{
			// увеличиваем время, которое длится анимация на входную дельту
			_currentAnimationTime += deltaTime;

			// пока текущее время анимации больше чем, время, которое должен длится текущий кадр анимации
			while (_currentAnimationTime >= _pCurrentAnimationDurations->second[_currentFrame].second)
			{
				// уменьшаем текущее время на время длительности текущего кадра
				_currentAnimationTime -= _pCurrentAnimationDurations->second[_currentFrame].second;
				// перещелкиваем кадр
				++_currentFrame;
				// устанавливаем флаг, который разрешает перерисовку
				_dirty = true;
				// если дошли до последнего кадра, начинаем анимацию с начала
				if (_currentFrame == _pCurrentAnimationDurations->second.size())
				{
					_currentFrame = 0;
				}
			}
		}
	}
	// установка состояния, которое хотим отрисовывать
	void AnimatedSprite::setState(std::string newState)
	{
		auto it = _statesMap.find(newState);
		if (it == _statesMap.end())
		{
			std::cerr << "can not find animation state: " << newState << std::endl;
			return;
		}
		// если новое состояние, не то же самое, что было, перещелкиваем все в ноль
		if (it != _pCurrentAnimationDurations)
		{
			_currentAnimationTime = 0;
			_currentFrame = 0;
			_pCurrentAnimationDurations = it;
			_dirty = true;
		}
	}
}