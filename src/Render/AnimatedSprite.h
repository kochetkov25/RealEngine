/*
**  File        :	AnimatedSprite.h
**  Authors     :   Kochetkov K.I.
**  Created on  :   08.03.2024
**  Modified on :   08.03.2024
**  Description :
*/

#pragma once

#include "Sprite.h"

#include <memory>

namespace Render
{

	class AnimatedSprite
	{
	public:
		/*конструктор*/
		AnimatedSprite(std::shared_ptr<Sprite> pSprite);

		/*получить текущий кадр анимации*/
		size_t getCurrentFrame() const
		{
			return _currFrame;
		}

		/*обновить состояние анимированного спрайта*/
		void update(const uint64_t del);

		/*отрисовка спрайта*/
		void render();
	private:
		std::shared_ptr<Sprite> _pSprite;
		size_t _currFrame;
		uint64_t _currFrameDuration;
		uint64_t _currAnimTime;
	};


}
